#include <Arduino.h>
#include "rc.h"
#include <VNH3SP30.h>

#define DRIVE_PWM 9  // pwm pin motor
#define DRIVE_INA 7  // control pin INA
#define DRIVE_INB 8  // control pin INB
#define DRIVE_DIAG 0 // diagnose pins (combined DIAGA/ENA and DIAGB/ENB)
#define DRIVE_CS 0   // current sense pin

#define STEERING_PWM 10 // pwm pin motor
#define STEERING_INA 11 // control pin INA
#define STEERING_INB 12 // control pin INB
#define STEERING_DIAG 0 // diagnose pins (combined DIAGA/ENA and DIAGB/ENB)
#define STEERING_CS 0   // current sense pin

unsigned long now; // timing variables to update data at a regular interval
unsigned long rc_update;
const int channels = 6; // specify the number of receiver channels
int RC_in[channels];    // an array to store the calibrated input from receiver

bool valid_values;
VNH3SP30 drive_motor;    // define control object for 1 motor
VNH3SP30 steering_motor; // define control object for 1 motor

void setup()
{
  setup_pwmRead();
  Serial.begin(9600);
  drive_motor.begin(DRIVE_PWM, DRIVE_INA, DRIVE_INB, DRIVE_DIAG, DRIVE_CS);                   // Motor 1 object connected through specified pins
  steering_motor.begin(STEERING_PWM, STEERING_INA, STEERING_INB, STEERING_DIAG, STEERING_CS); // Motor 1 object connected through specified pins
  drive_motor.setSpeed(0);                                                                    // motor stop
  steering_motor.setSpeed(0);                                                                 // motor stop
}

void loop()
{

  now = millis();

  if (RC_avail() || now - rc_update > 25)
  { // if RC data is available or 25ms has passed since last update (adjust to be equal or greater than the frame rate of receiver)
    valid_values = true;
    rc_update = now;

    // print_RCpwm();                        // uncommment to print raw data from receiver to serial

    for (int i = 0; i < channels; i++)
    { // run through each RC channel
      int CH = i + 1;

      RC_in[i] = RC_decode(CH); // decode receiver channel and apply failsafe
      if (RC_in[i] == 0)
      {
        valid_values = false;
      }
      // print_decimal2percentage(RC_in[i]);   // uncomment to print calibrated receiver input (+-100%) to serial
    }
    // Serial.println();                       // uncomment when printing calibrated receiver input to serial.
  }
  else
  {
    // valid_values = false;
  }

  if ((RC_in[0] != 0) && (RC_in[1] != 0))
  {
    Serial.print("-> ");
    Serial.print(valid_values);
    Serial.print(" ");
    Serial.print(RC_in[0]);
    Serial.print(" ");
    Serial.print(RC_in[1]);
    Serial.print(" ");
    int drive_speed = map((int)RC_in[0], 1000, 2000, -400, 400);
    if (drive_speed > 400)
    {
      drive_speed = 400;
    }
    if (drive_speed < -400)
    {
      drive_speed = -400;
    }

    int steering_speed = map((int)RC_in[1], 1000, 2000, -400, 400);
    if (steering_speed > 400)
    {
      steering_speed = 400;
    }
    if (steering_speed < -400)
    {
      steering_speed = -400;
    }

    Serial.print(drive_speed);
    Serial.print(" ");
    Serial.print(steering_speed);
    Serial.println();

    drive_motor.setSpeed(drive_speed);
    steering_motor.setSpeed(steering_speed);
  }
  else
  {
    drive_motor.setSpeed(0);
    steering_motor.setSpeed(0);
  }
  // drive_motor.setSpeed(400);
  // steering_motor.setSpeed(400);
}