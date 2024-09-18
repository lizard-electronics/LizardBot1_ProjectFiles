#ifndef _ENCODER_DRIVER_H_
#define _ENCODER_DRIVER_H_

#include <Arduino.h>
#include <definitions.h>
#include <esp32_pinout.h>
#include <commands.h>

/*****************************************************************
* Encoder driver function definitions
* - original by James Nugen (modified by Manuel Lagarto)
*****************************************************************/   
long readEncoder(int i);
void resetEncoder(int i);
void resetEncoders();

/* Encoder ISR functions declaration*/
void IRAM_ATTR handleMotor1Encoder();
void IRAM_ATTR handleMotor2Encoder();

/* Expose access to encoder values */
extern volatile long left_enc_pos;
extern volatile long right_enc_pos;

#endif