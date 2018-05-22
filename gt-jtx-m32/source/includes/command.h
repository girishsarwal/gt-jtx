/*
 * command.h
 *
 *  Created on: 22-May-2018
 *      Author: girishsarwal
 */

#ifndef INCLUDES_COMMAND_H_
#define INCLUDES_COMMAND_H_

enum OPCODE {
   NOP = 0x00,        /** a NOP request will send back the status code to master, if no error, then NOP will be sent back **/
   RESET = 0xFF,	  /** 0xFF is unique code. When gt-jtx gets this signal, it will cause a reset on all values akin to a boot**/
};

#endif /* INCLUDES_COMMAND_H_ */
