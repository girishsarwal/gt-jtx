/*
 * model.h
 *
 *  Created on: 07-Jan-2020
 *      Author: girishsarwal
 */

#ifndef INCLUDE_MODEL_H_
#define INCLUDE_MODEL_H_
#include "common.h"
#include "mix.h"
#define MAX_LEN_NAME 16
#define MAX_MIXES 128

class Model {
public:
	Model();
	virtual ~Model();

	char m_Name[MAX_LEN_NAME];			/** name of the model **/
	uint8_t m_Uuid[4];					/** 4 bytes of uuid **/
	Mix m_Mixes[MAX_MIXES];				/** array of mixes **/
	uint8_t m_Trims[NUM_ANALOG_INPUTS];	/** array of trims **/

public:
	void saveTrim(uint8_t channel);		/** updates the trim to a channel and the eeprom**/
	void serialize();					/** writes the model to the eeprom **/
	void deserialize();					/** reads the model from the eeprom **/
};

#endif /* INCLUDE_MODEL_H_ */
