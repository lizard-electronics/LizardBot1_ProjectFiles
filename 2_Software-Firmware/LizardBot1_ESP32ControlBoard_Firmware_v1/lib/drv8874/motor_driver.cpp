#include <motor_driver.h>

int left_speed = 0;
int right_speed = 0;

void initMotorController() {
   digitalWrite(MOTOR1_nSLEEP, HIGH);
   digitalWrite(MOTOR2_nSLEEP, HIGH);
}

void setMotorSpeed(int i, int spd) {
   unsigned char reverse = 0;

   if(spd < 0) {
      spd = -spd;
      reverse = 1;
   }
   if(spd > 255)
      spd = 255;
   
   if(i == LEFT) { 
      if(reverse == 0) {
         analogWrite(MOTOR2_IN2, spd);
         analogWrite(MOTOR2_IN1, 0);
      }
      else if(reverse == 1) {
         analogWrite(MOTOR2_IN1, spd);
         analogWrite(MOTOR2_IN2, 0);
      }
   }
   else /*if (i == RIGHT)*/ {
      if(reverse == 0) {
         analogWrite(MOTOR1_IN1, spd);
         analogWrite(MOTOR1_IN2, 0);
      }
      else if(reverse == 1) {
         analogWrite(MOTOR1_IN2, spd);
         analogWrite(MOTOR1_IN1, 0);
      }
   }
}

void setMotorSpeeds(int leftSpeed, int rightSpeed) {
   left_speed = leftSpeed;
   right_speed = rightSpeed;
   setMotorSpeed(LEFT, leftSpeed);
   setMotorSpeed(RIGHT, rightSpeed);
}
