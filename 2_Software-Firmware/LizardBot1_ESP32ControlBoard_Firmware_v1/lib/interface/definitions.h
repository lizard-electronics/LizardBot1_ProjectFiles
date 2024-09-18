#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

/* Serial port baud rate */
#define BAUD_RATE 115200

/* Maximum PWM signal */
#define MAX_PWM 255

/* Run the PID loop at 30 times per second */
#define PID_RATE 30

/* Stop the robot if it hasn't received a movement command (ms) */
#define AUTO_STOP_INTERVAL 2000

#endif