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
   SETTUP = 0x01,
   SETTDN = 0x02,
   GETT = 0x03,
   SETREV = 0x04,
   GETREV = 0x05,
   SCUP = 0x06,
   GCUP = 0x07,
   SCDN = 0x08,
   GCDN = 0x09,
   SPPMLEN = 0x0A,
   GPPMLEN = 0x0B,
   SPPMICL = 0x0C,
   GPPMICL = 0x0D,
   SSTIM = 0x0E,
   GSTIM = 0x0F,
   GCV = 0x10,
   RESET = 0xFF,	  /** 0xFF is unique code. When gt-jtx gets this signal, it will cause a reset on all values akin to a soft reboot**/
};

#endif /* INCLUDES_COMMAND_H_ */
