#include <Arduino.h>
#include "esp32_pinout.h"
#include "definitions.h"

/*****************************************************************
* Setup function runs once at startup.
*****************************************************************/
void setup() {
   // initialize serial communication
   Serial.begin(115200);
   Serial.write("\nHello from LizardBot1 Control Board");

   // initialize output pins
   pinMode(LED, OUTPUT);
   pinMode(BUZZER, OUTPUT);
   pinMode(MOTOR1_IN1, OUTPUT);
   pinMode(MOTOR1_IN2, OUTPUT);  
   pinMode(MOTOR1_nSLEEP, OUTPUT);
   pinMode(MOTOR2_IN1, OUTPUT);
   pinMode(MOTOR2_IN2, OUTPUT);  
   pinMode(MOTOR2_nSLEEP, OUTPUT);
   Serial.write("\nOutput pins initialized");
}

/*****************************************************************
* Enter the main loop.  Read and parse input from the serial port
* and run any valid commands. Run a PID calculation at the target
* interval and check for auto-stop conditions.
*****************************************************************/
void loop() {
  
}
