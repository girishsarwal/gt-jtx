/*
 * runtime.h
 *
 * The runtime is a "virtual machine" that's executed on
 * the dedicated controller. It could be an AVR or the STM // TODO
 * It does not store any model information or any other auxillary info
 * but only a basic set of parameters required to
 * generate the cppm signal to be fed to the RF circuitry
 *
 * The two members MODEL and SETTINGS are simply used for
 * encapsulation and are independent of the client consuming
 *  Created on: 07-Jan-2020
 *      Author: Girish Sarwal
 *      Email:	girish.sarwal@gluedtomatoes.com
 *      		g.sarwal@gmail.com
 */

#ifndef INCLUDE_RUNTIME_H_
#define INCLUDE_RUNTIME_H_
class Usart;
#include "common.h"
#include "settings.h"
#include "model.h"
#include "usart.h"
class Runtime {
private:
	uint8_t idx;
public:
	Runtime();
	virtual ~Runtime();

	uint16_t m_PhysicalInputs[NUM_PHYSICAL_INPUTS];
	uint16_t m_Cppm[NUM_OUTPUTS_INCLUDING_SYNC];

	uint16_t 	m_SignalTraversalUs;
	uint16_t 	m_MidSignalWidthUs;
	uint16_t 	m_SyncSignalWidthUs;
	uint8_t 	m_LastError;			/** startup errors **/
	uint8_t 	m_Currentchannel;

public:
	Settings* m_settings;				/** serialized to eeprom **/
	Model* m_activeModel;				/** serialized to eeprom **/
private:
	Usart* m_usart;						/** the usart controller **/
public:
	void reset();
	void initialize();
	void setupHardware();
	void calculateSignalParameters();
	void readAnalogInput(uint8_t);
	void readDigitalInput(uint8_t, uint8_t);
	void incrementTrim(uint8_t);
	void decrementTrim(uint8_t);
	void calibrateChannel(uint8_t);
	void enableCppm();
	void disableCppm();


	void processTrimInput();
	void serialize();
	void deserialze();
	void loadOrDefault();

private:
	uint16_t readAnalog(uint8_t);
	uint8_t getKeyPressed(uint8_t, uint8_t);

	void writeFrameToReceiveQueue(/** FRAME* frame **/);
	void readFrameFromReceiveQueue(/** FRAME * frame **/);
	void writeFrameToSendQueue(/** FRAME* frame **/);
	void readFrameFromSendQueue(/** FRAME * frame **/);

	void run();

	//FRAME_BUFFER* receive_queue;		/** receive queue from front end **/
	//FRAME_BUFFER* send_queue;		/** send queue to front end **/

	//	uint8_t		usart_state;

	/** from here on everything will be serialized to eeprom so it gets loaded on the next bootup **/

};

#endif /* INCLUDE_RUNTIME_H_ */
