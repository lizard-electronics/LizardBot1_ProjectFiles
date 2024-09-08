#include <Arduino.h>

const int LED_GPIO2 = 2;
const int BUZZER_GPIO14 = 14;

const int MOTOR1_IN1 = 18;
const int MOTOR1_IN2 = 17;
const int MOTOR1_nSLEEP = 16;

const int MOTOR2_IN1 = 27;
const int MOTOR2_IN2 = 26;
const int MOTOR2_nSLEEP = 25;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_GPIO2, OUTPUT);
  pinMode(BUZZER_GPIO14, OUTPUT);
  pinMode(MOTOR1_IN1, OUTPUT);
  pinMode(MOTOR1_IN2, OUTPUT);  
  pinMode(MOTOR1_nSLEEP, OUTPUT);
  pinMode(MOTOR2_IN1, OUTPUT);
  pinMode(MOTOR2_IN2, OUTPUT);  
  pinMode(MOTOR2_nSLEEP, OUTPUT);

  Serial.begin(115200);  
  Serial.write("\nHello from LizardBot1 Control Board (ESP32)");

  //tone(BUZZER_GPIO14, 1000); // gives LEDC is not initialized error in PlatformIO (use ESP32 LEDC API)
  digitalWrite(LED_GPIO2, HIGH);
  delay(250);
  //noTone(BUZZER_GPIO14);
  digitalWrite(LED_GPIO2, LOW);

  // set the motor speed to half of the maximum (254)
  int motor_speed = 127;
  int rotation_state = 0;

  // enable motor drivers
  digitalWrite(MOTOR1_nSLEEP, HIGH);
  digitalWrite(MOTOR2_nSLEEP, HIGH);

  while(rotation_state < 4) {
    switch(rotation_state) {
      case 0:
        // Start the motor 1
        analogWrite(MOTOR1_IN1, motor_speed);
        analogWrite(MOTOR1_IN2, 0);
        Serial.write("\nStart the motor 1");
        break;

      case 1:      
        // Start the motor 2
        analogWrite(MOTOR2_IN2, motor_speed);
        analogWrite(MOTOR2_IN1, 0);
        Serial.write("\nStart the motor 2");
        break;

      case 2:      
        // Invert direction
        analogWrite(MOTOR1_IN1, 0);
        analogWrite(MOTOR1_IN2, motor_speed);

        analogWrite(MOTOR2_IN2, 0);
        analogWrite(MOTOR2_IN1, motor_speed);
        
        Serial.write("\nInvert direction");
        break;

      case 3:      
        // Stop the motor
        analogWrite(MOTOR1_IN2, 0);
        analogWrite(MOTOR2_IN1, 0);
        Serial.write("\nStop the motor");
        break;

      default:
        break;
    }
    delay(2000);
    rotation_state++;
  }

  // disable motor drivers
  digitalWrite(MOTOR1_nSLEEP, LOW);
  digitalWrite(MOTOR2_nSLEEP, LOW);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_GPIO2, HIGH);
  delay(250);
  digitalWrite(LED_GPIO2, LOW);
  delay(250);
}
