/*
 * mix.h
 *
 *  Created on: 22-May-2018
 *      Author: girishsarwal
 */

#ifndef INCLUDES_MIX_H_
#define INCLUDES_MIX_H_
#include "common.h"
/** mix **/
/** everything is a mix, we use the nodegraph editor to build these
 * dr - dual rates,
 * reversal - reverse the signal direction
 * weight - the final weight of the input
 * expo - expnential,
 * offset - channel value will be offset by this
 * delay - channel value will be delayed by this amount before the command executes
 * blip - channel will hesitate progress defined by the blip, will use a lerp to interpolate
 *
 */
typedef struct {
	uint8_t source;			/** source that transform the value, index into the input_hw_controls array **/
	uint8_t target;			/** target that will be changed, values will be first read and then modified based on mplex and rev
										 , always an index in the output_ppm array **/
} MIX;

#endif /* INCLUDES_MIX_H_ */
