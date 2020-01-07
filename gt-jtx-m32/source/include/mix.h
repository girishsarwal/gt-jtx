/*
 * mix.h
 *
 *
 * everything is a mix, we use a nodegraph type editor to build these
 *
 * source - source that transform the value, index into the input_hw_controls array
 * target - the index into the output_hw_controls array that will be changed.
 *
 * dr - dual rates,
 * reversal - reverse the signal direction
 * weight - the final weight of the input
 * expo - expnential,
 * offset - channel value will be offset by this
 * delay - channel value will be delayed by this amount before the command executes
 * blip - channel will hesitate progress defined by the blip, will use a lerp to interpolate
 *
 *
 *  Created on: 07-Jan-2020
 *      Author: Girish Sarwal
 *      Email:	girish.sarwal@gluedtomatoes.com
 *      		g.sarwal@gmail.com
 */

#ifndef INCLUDE_MIX_H_
#define INCLUDE_MIX_H_
#include "common.h"
class Mix {
public:
	Mix();
	Mix(uint8_t source);
	virtual ~Mix();
private:
	uint8_t source;			/** channel that will be affected **/
private:
							/** the mix will be applied if { trigger operation condition } evaluates to true **/
	uint8_t m_trigger;		/** trigger, this will be one out of NUM_INPUTS **/
	uint8_t m_operation;	/** the operation to be applied **/
	uint8_t m_condition;	/** the values that needs to be tested against **/

public:
	uint8_t m_expo;			/** the value will be interpolated based on the expo function **/
	uint8_t m_rev_mplex;	/** D0 - rev/inh, D1-D2 - indicates multiplex (REPLACE, ADD, MULTIPLY) **/
	uint8_t m_weight;		/** output will be multipled by this weight % at the final stage **/
	uint8_t m_delay;		/** delay (s) in which the mix should be applied, 0 is immediate**/
public:
	void apply();
};

#endif /* INCLUDE_MIX_H_ */
