#include "encoder_driver.h"

volatile long left_enc_pos = 0L;
volatile long right_enc_pos = 0L;
static const int8_t ENC_STATES [] = {0,1,-1,0,-1,0,0,1,1,0,0,-1,0,-1,1,0};  //encoder lookup table
   
/* Interrupt routine for MOTOR1 encoder, taking care of actual counting */
void IRAM_ATTR handleMotor1Encoder() {
   static uint8_t enc_last=0;

   //shift previous state two places
   enc_last <<=2;
   //read the current state into lowest 2 bits
   enc_last |= (digitalRead(MOTOR1_ENC_B) << 1) | digitalRead(MOTOR1_ENC_A);

   right_enc_pos += ENC_STATES[(enc_last & 0x0f)];
}

/* Interrupt routine for MOTOR2 encoder, taking care of actual counting */
void IRAM_ATTR handleMotor2Encoder() {
   static uint8_t enc_last=0;

   //shift previous state two places
   enc_last <<=2;
   //read the current state into lowest 2 bits
   enc_last |= (digitalRead(MOTOR2_ENC_A) << 1) | digitalRead(MOTOR2_ENC_B);

   left_enc_pos += ENC_STATES[(enc_last & 0x0f)];
}

/* Wrap the encoder reading function */
long readEncoder(int i) {
   if (i == LEFT) return left_enc_pos;
   else return right_enc_pos;
}

/* Wrap the encoder reset function */
void resetEncoder(int i) {
   if (i == LEFT){
      left_enc_pos=0L;
      return;
   } else { 
      right_enc_pos=0L;
      return;
   }
}

/* Wrap the encoder reset function */
void resetEncoders() {
   resetEncoder(LEFT);
   resetEncoder(RIGHT);
}
