#include <Arduino.h>
#include <definitions.h>
#include <esp32_pinout.h>
#include <commands.h>
#include <diff_controller.h>
#include <encoder_driver.h>
#include <motor_driver.h>

/*****************************************************************
* Variable initialization
*****************************************************************/
// A pair of varibles to help parse serial commands (thanks Fergs)
int arg = 0;
int i = 0;

// Variable to hold an input character
char chr;

// Variable to hold the current single-character command
char cmd;

// Character arrays to hold the first and second arguments
char argv1[16];
char argv2[16];

// The arguments converted to integers
long arg1;
long arg2;

/* Convert the rate into an interval */
const int PID_INTERVAL = 1000 / PID_RATE;

/* Track the next time we make a PID calculation */
unsigned long nextPID = PID_INTERVAL;

long lastMotorCommand = AUTO_STOP_INTERVAL;


/*****************************************************************
* Functions
*****************************************************************/
/* Clear the current command parameters */
void resetCommand() {
   cmd = '\0';
   memset(argv1, 0, sizeof(argv1));
   memset(argv2, 0, sizeof(argv2));
   arg1 = 0;
   arg2 = 0;
   arg = 0;
   i = 0;
}

/* Run a command.  Commands are defined in commands.h */
void runCommand() {
   int i = 0;
   char *p = argv1;
   char *str;
   int pid_args[4];
   arg1 = atoi(argv1);
   arg2 = atoi(argv2);
   float pin_value = 0;
   float pin_voltage = 0;
   float batt_voltage = 0;

   switch(cmd) {
      case GET_BAUDRATE:
         Serial.println(BAUD_RATE);
         break;

      case READ_BATT:
         // calculate battery voltage based on the voltage divider
         pin_value = analogRead(BATT_STATUS);
         pin_voltage = (3.3/4095)*pin_value;
         batt_voltage = (pin_voltage/3300)*(3300+10000);
         // show
         Serial.print(pin_value);
         Serial.print(" ");
         Serial.print(pin_voltage);
         Serial.print(" ");
         Serial.println(batt_voltage);
         break;

      case ANALOG_READ:
         Serial.println(analogRead(arg1));
         break;

      case DIGITAL_READ:
         Serial.println(digitalRead(arg1));
         break;

      case ANALOG_WRITE:
         analogWrite(arg1, arg2);
         Serial.println("OK"); 
         break;

      case DIGITAL_WRITE:
         if (arg2 == 0) digitalWrite(arg1, LOW);
         else if (arg2 == 1) digitalWrite(arg1, HIGH);
         Serial.println("OK"); 
         break;
      
      case READ_ENCODERS:
         Serial.print(readEncoder(LEFT));
         Serial.print(" ");
         Serial.println(readEncoder(RIGHT));
         break;

      case RESET_ENCODERS:
         resetEncoders();
         resetPID();
         Serial.println("OK");
         break;

      case MOTOR_RAW_PWM:
         /* Reset the auto stop timer */
         lastMotorCommand = millis();
         resetPID();
         moving = 0; // Sneaky way to temporarily disable the PID
         setMotorSpeeds(arg1, arg2);
         Serial.println("OK");
         break;

      case MOTOR_SPEEDS:
         /* Reset the auto stop timer */
         lastMotorCommand = millis();
         if (arg1 == 0 && arg2 == 0) {
            setMotorSpeeds(0, 0);
            resetPID();
            moving = 0;
         }
         else moving = 1;
         leftPID.TargetTicksPerFrame = arg1;
         rightPID.TargetTicksPerFrame = arg2;
         Serial.println("OK"); 
         break;

      case UPDATE_PID:
         while ((str = strtok_r(p, ":", &p)) != nullptr) {
            pid_args[i] = atoi(str);
            i++;
         }
         Kp = pid_args[0];
         Kd = pid_args[1];
         Ki = pid_args[2];
         Ko = pid_args[3];
         Serial.println("OK");
         break;
      
      default:
         Serial.println("Invalid Command");
         break;
      }
}


/*****************************************************************
* Setup function runs once at startup.
*****************************************************************/
void setup() {
   // initialize serial communication
   Serial.begin(115200);
   Serial.print("\nHello from LizardBot1 Control Board");

   // initialize output pins
   pinMode(LED, OUTPUT);
   pinMode(BUZZER, OUTPUT);
   pinMode(MOTOR1_IN1, OUTPUT);
   pinMode(MOTOR1_IN2, OUTPUT);  
   pinMode(MOTOR1_nSLEEP, OUTPUT);
   pinMode(MOTOR2_IN1, OUTPUT);
   pinMode(MOTOR2_IN2, OUTPUT);  
   pinMode(MOTOR2_nSLEEP, OUTPUT);
   Serial.print("\nOutput pins initialized");

   // declare interrupt pins
   attachInterrupt(digitalPinToInterrupt(MOTOR1_ENC_A), handleMotor1Encoder, CHANGE);
   attachInterrupt(digitalPinToInterrupt(MOTOR1_ENC_B), handleMotor1Encoder, CHANGE);
   attachInterrupt(digitalPinToInterrupt(MOTOR2_ENC_A), handleMotor2Encoder, CHANGE);
   attachInterrupt(digitalPinToInterrupt(MOTOR2_ENC_B), handleMotor2Encoder, CHANGE);
   Serial.print("\nInterrupt pins declared");
   Serial.println("\nSystem started. Wainting input...");


   initMotorController();
   resetPID();
}


/*****************************************************************
* Enter the main loop.  Read and parse input from the serial port
* and run any valid commands. Run a PID calculation at the target
* interval and check for auto-stop conditions.
*****************************************************************/
void loop() {
   while (Serial.available() > 0) {    
      // Read the next character
      chr = Serial.read();

      // Terminate a command with a CR
      if (chr == 13) {
         if (arg == 1) argv1[i] = '\0';
         else if (arg == 2) argv2[i] = '\0';
         runCommand();
         resetCommand();
      }
      // Use spaces to delimit parts of the command
      else if (chr == ' ') {
         // Step through the arguments
         if (arg == 0) arg = 1;
         else if (arg == 1)  {
            argv1[i] = '\0';
            arg = 2;
            i = 0;
         }
         continue;
      }
      else {
         if (arg == 0) {
            // The first arg is the single-letter command
            cmd = chr;
         }
         else if (arg == 1) {
            // Subsequent arguments can be more than one character
            argv1[i] = chr;
            i++;
         }
         else if (arg == 2) {
            argv2[i] = chr;
            i++;
         }
      }
   }
  
   if (millis() > nextPID) {
      updatePID();
      nextPID += PID_INTERVAL;
   }

   // Check to see if we have exceeded the auto-stop interval
   if ((millis() - lastMotorCommand) > AUTO_STOP_INTERVAL) {
      setMotorSpeeds(0, 0);
      moving = 0;

      lastMotorCommand = millis();   // (*) strange bug if removed
      // (*) if removed, when auto-stop interval is reached
      // motors are set to full PWM (???)
      // i.e. when sending "o 60 60", auto-stop interval
      // doesn't stop the motors (value needs to be >0)
   }
}
