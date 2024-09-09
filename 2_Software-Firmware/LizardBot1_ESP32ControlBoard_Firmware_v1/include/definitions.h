#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

/* Serial port baud rate */
#define BAUD_RATE 115200

/* Maximum PWM signal */
#define MAX_PWM 255

/* Run the PID loop at 30 times per second */
#define PID_RATE 30

/* Convert the rate into an interval */
const int PID_INTERVAL = 1000 / PID_RATE;

/* Track the next time we make a PID calculation */
unsigned long nextPID = PID_INTERVAL;

/* Stop the robot if it hasn't received a movement command (ms) */
#define AUTO_STOP_INTERVAL 2000
long lastMotorCommand = AUTO_STOP_INTERVAL;

#endif