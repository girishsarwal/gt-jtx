/*
 * runtime.cpp
 *
 *  Created on: 07-Jan-2020
 *      Author: girishsarwal
 */

#include "../include/runtime.h"

Runtime::Runtime() {
	m_settings = new Settings();
	m_activeModel = new Model();
	m_usart = new Usart();

	/** attempt to load settings from eeprom **/
}

Runtime::~Runtime() {
	// TODO Auto-generated destructor stub
}
void Runtime::reset() {
	idx = -1;
	/** reset the cppm to center **/
	memset16(m_Cppm, m_MidSignalWidthUs, NUM_OUTPUTS_INCLUDING_SYNC);
	/**reset the sync signal **/
	m_Cppm[SYNC] = m_SyncSignalWidthUs;
	/** reset to the first channel **/
	m_Currentchannel = CH1;
	/** usart status	 */
	m_usart->setRuntime(this);
	m_usart->reset();
}
void Runtime::setupHardware(){
	/** Analog Inputs */
	DDR_ANALOG = 0x00;
	PORT_ANALOG = 0x00;

	/** Digital Inputs */
	DDR_DIGITAL = 0x00;
	PORT_DIGITAL = 0x00;

	/** Trim switches */
	DDR_TRIM = 0x00;
	PORT_TRIM = 0xFF;

	/** setup the usart */
	UBRRL = UBRRVAL;
	UBRRH = (UBRRVAL>>8);
	UCSRC=(1<<URSEL)|(0<<UMSEL)|(0<<UPM1)|(0<<UPM0)|
			(0<<USBS)|(0<<UCSZ2)|(1<<UCSZ1)|(1<<UCSZ0);
	UCSRB=(1<<RXEN)|(1<<TXEN)|(1<<RXCIE);

	/** Setup PPM generator */
	TCNT1 = 0;
	TCCR1B |= (1<<CS10) ;									/** F_CPU - 1Mhz Timebase **/
	TCCR1B	|=	(1<<WGM12);									/** CTC Mode**/
	TCCR1A |= (1<<COM1A0);									/** hardware ctc **/

	/** Enable the ADC to read analog channels */
	ADMUX |= (1<<REFS0);
	ADCSRA|= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);			/**F_CPU/64 Prescalar **/
	ADCSRA |= (1<<ADEN); 									/** enable the ADC **/
}

void Runtime::calculateSignalParameters(){
	m_MidSignalWidthUs = ((m_settings->m_MinSignalWidthUs + m_settings->m_MaxSignalWidthUs)/2) + TRIM_CENTER;
	m_SyncSignalWidthUs = (m_settings->m_FrameWidthUs - (NUM_OUTPUTS * (m_MidSignalWidthUs + m_settings->m_InterChannelWidthUs))); /**whatever is left **/
	m_SignalTraversalUs = m_settings->m_MaxSignalWidthUs - m_settings->m_MinSignalWidthUs;
}


/*
** This method reads up the analog value of a channel to the ppm array
** the first 8 channels can have either analog or digital values.
** the next 8 channels can only have digital values.
** analog or digital depends on whether one puts a POT ot a SPDT switch on that channel
**/
void Runtime::readAnalogInput(uint8_t ch) {
	uint16_t value = readAnalog(ch);
	/** perform a lerp to get the moved value **/
	float calibratedChannelRange = m_settings->m_UpperCalibrations[ch] - m_settings->m_LowerCalibrations[ch];
	float stickMoveRatio = (value - m_settings->m_LowerCalibrations[ch])/calibratedChannelRange;
	/** TODO: We're moving to the signal width here directly,
	 * we should just get a travel ratio
	 * the actual conversion to us should be at the end, why here?
	 */
	m_PhysicalInputs[ch] = m_settings->m_MinSignalWidthUs + ((stickMoveRatio * (float)m_SignalTraversalUs)) + m_activeModel->m_Trims[ch];
}


/**************************************** read_switch *****************************
** This method reads up the digital value of a digital input (D0-D7) into the
** correct hardware channels. These inputs can only accept a digital switch
**
** uint8_t ch - the channel number in the inputs array
** uint8_t - the signal which will be read
**/
void Runtime::readDigitalInput(uint8_t ch, uint8_t pin) {
	volatile uint8_t value = getKeyPressed(PORT_DIGITAL, pin);
	/** TODO: reversal? **/
	//volatile uint8_t value = (get_key_pressed(port, pin) ^ ((reverse & mask) == mask));  /** reverse is not a mixes feature. program it there **/
	/** TODO: We're moving to the signal width here directly,
	 * we should just get a travel ratio
	 * the actual conversion to us should be at the end, why here?
	 */
	m_PhysicalInputs[ch] = m_settings->m_MinSignalWidthUs + (value * (float)m_SignalTraversalUs);
}

/**************************************** increment_trim *****************************
** This method increments the trim values of a channel and clamps them between the min and max
** trim values
**/
void Runtime::incrementTrim(uint8_t ch){
	if(++(m_activeModel->m_Trims[ch]) > TRIM_UPPER_END){
		(m_activeModel->m_Trims[ch]) = TRIM_UPPER_END;
	}
	m_activeModel->saveTrim(ch);
}
/**************************************** decrement_trim *****************************
** This method increments the trim values of a channel and clamps them between the min and max
** trim values
**/
void Runtime::decrementTrim(uint8_t ch){
	if(m_activeModel->m_Trims[ch] > 0 && --m_activeModel->m_Trims[ch]<= TRIM_LOWER_END){
		m_activeModel->m_Trims[ch] = TRIM_LOWER_END;
	}
	m_activeModel->saveTrim(ch);
}

/**************************************** calibrate_channel *****************************
** This method keeps on updating the values if the calibration settings of a channel
** call the method in a loop
**/

void Runtime::calibrateChannel(uint8_t ch) {
	uint16_t calibration = readAnalog(ch);
	if(m_settings->m_UpperCalibrations[ch] < calibration)
		m_settings->m_UpperCalibrations[ch] = calibration;
	if(m_settings->m_LowerCalibrations[ch] > calibration)
		m_settings->m_LowerCalibrations[ch] = calibration;
}

/*****************************************  process_key_inputs ****************************************
	* processes Key inputs on the gt-jtx. These are trim keys only for the gt-jtx-m32
	* most of controls elsewise will be via SPI
	* this needs to be called in a loop for continous polling. Call in the main loop
**/

void Runtime::initialize() {
	setupHardware();
	calculateSignalParameters();
	reset();
	enableCppm();
}

void Runtime::enableCppm(){

	/**Enable global interrupts **/
	sei();
	/** start our timer **/
	TIMSK |= (1<<OCIE1A);
	/** enable the adc **/
	ADCSRA |= (1<<ADSC);
}

void Runtime::disableCppm() {

}
void Runtime::run() {
	while(1){
		/** read analog inputs **/
		readAnalogInput(AI0);
		readAnalogInput(AI1);
		readAnalogInput(AI2);
		readAnalogInput(AI3);
		readAnalogInput(AI4);
		readAnalogInput(AI5);
		readAnalogInput(AI6);
		readAnalogInput(AI7);

		/** read digital inputs **/
		readDigitalInput(INP8, DI0);
		readDigitalInput(INP9, DI1);
		readDigitalInput(INP10, DI2);
		readDigitalInput(INP11, DI3);
		readDigitalInput(INP12, DI4);
		readDigitalInput(INP13, DI5);
		readDigitalInput(INP14, DI6);
		readDigitalInput(INP15, DI7);

		/** check if any trim keys have been pressed **/
		if(getKeyPressed(PIN_TRIM, SIG_TRIM_CH1_PLUS)) incrementTrim(CH1);
		if(getKeyPressed(PIN_TRIM, SIG_TRIM_CH1_MINUS)) decrementTrim(CH1);
		if(getKeyPressed(PIN_TRIM, SIG_TRIM_CH2_PLUS)) incrementTrim(CH2);
		if(getKeyPressed(PIN_TRIM, SIG_TRIM_CH2_MINUS)) decrementTrim(CH2);
		if(getKeyPressed(PIN_TRIM, SIG_TRIM_CH3_PLUS)) incrementTrim(CH3);
		if(getKeyPressed(PIN_TRIM, SIG_TRIM_CH3_MINUS)) decrementTrim(CH3);
		if(getKeyPressed(PIN_TRIM, SIG_TRIM_CH4_PLUS)) incrementTrim(CH4);
		if(getKeyPressed(PIN_TRIM, SIG_TRIM_CH4_MINUS)) decrementTrim(CH4);

		/** apply the mixes **/
//			uint8_t idx_mixes = -1;
//			while(++idx_mixes < MAX_MIXES){
//				mix_apply_transform(&(runtime.model.mixes[idx_mixes]));
//			};

		/** process the receive frame queue **/
		/** process the transmit frame queue **/
	}
}

void runtime_process_usart_rx() {
	//
	//switch(runtime.usart_state){
	//		case WAIT_HEADER:
	//			if(FRAME_START == data){
	//				runtime.usart_state = IN_MSG;
	//				/** reset the buffer **/
	//			}
	//			break;
	//		case IN_MSG:
	//			if(DLE == data){
	//				runtime.usart_state = AFTER_ESC;
	//			} else if (FRAME_END == data){
	//				runtime.usart_state = WAIT_HEADER;
	//				/** reset the buffer **/
	//			} else {
	//				/** just add to the buffer **/
	//			}
	//			break;
	//		case AFTER_ESC:
	//			data = SUBS(data);	/** substitute the data to get the original byte sent **/
	//			/** add the data to the buffer **/
	//			runtime.usart_state = IN_MSG;
	//			break;
	//	}
}



void Runtime::serialize() {
	m_settings->serialize();
	m_activeModel->serialize();
}
void Runtime::deserialze() {
	m_settings->deserialize();
	m_activeModel->deserialize();
}

void Runtime::writeFrameToReceiveQueue(){

}

void Runtime::readFrameFromReceiveQueue(){

}
void Runtime::writeFrameToSendQueue(){

}

void Runtime::readFrameFromSendQueue(){

}

uint16_t Runtime::readAnalog(uint8_t ch) {
	ch = ch & 0x07;					/** zero out all bits except last three for limiting channel **/
	ADMUX &=0xE0;					/**reset channel selection **/
	ADMUX |=ch;						/**select the requested channel **/
	ADCSRA |= (1<<ADSC);
	while(!(ADCSRA & (1<<ADIF)));
	ADCSRA|= (1<<ADIF);
	return (ADC);
};

/**************************************** get_key_pressed *****************************
** This method checks to see if a key has been pressed on a port.
** debounce is included
**/
uint8_t Runtime::getKeyPressed(uint8_t port, uint8_t key){
	if(((~port) & (1 << key)) == (1 << key))
	{
		_delay_ms(1);	/** get rid of keybounce **/
		return (((~port) & (1 << key)) == (1 << key));
	}
	return 0x00;
};

