/*
 * settings.h
 *
 *  Created on: 07-Jan-2020
 *      Author: Girish Sarwal
 *      Email:	girish.sarwal@gluedtomatoes.com
 *      		g.sarwal@gmail.com
 */

#ifndef INCLUDE_SETTINGS_H_
#define INCLUDE_SETTINGS_H_
#include "common.h"
class Settings {
public:
	Settings();
	virtual ~Settings();

	uint8_t m_setupState;
	uint16_t m_MinSignalWidthUs;
	uint16_t m_MaxSignalWidthUs;
	uint16_t m_InterChannelWidthUs;
	uint16_t m_FrameWidthUs;
	uint8_t num_hw_input;		/** why do we need this? **/
	uint16_t m_UpperCalibrations[NUM_ANALOG_INPUTS];
	uint16_t m_LowerCalibrations[NUM_ANALOG_INPUTS];
public:
	void serialize();					/** writes the settings to the eeprom **/
	void deserialize();					/** reads the settings from the eeprom **/
	static Settings* loadOrDefault();	/** provide a loaded setting or a default **/
};

#endif /* INCLUDE_SETTINGS_H_ */
