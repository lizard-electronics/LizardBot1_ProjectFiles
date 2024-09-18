#ifndef _MOTOR_DRIVER_H_
#define _MOTOR_DRIVER_H_

#include <Arduino.h>
#include <definitions.h>
#include <esp32_pinout.h>
#include <commands.h>

/*****************************************************************
* Motor driver function definitions
* - original by James Nugen (modified by Manuel Lagarto)
*****************************************************************/
void initMotorController();
void setMotorSpeed(int i, int spd);
void setMotorSpeeds(int leftSpeed, int rightSpeed);

extern int left_speed;
extern int right_speed;

#endif