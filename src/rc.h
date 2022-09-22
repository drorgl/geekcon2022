#pragma once

#include <Arduino.h>

void setup_pwmRead();
 boolean RC_avail();
 void print_RCpwm();
   float RC_decode(int CH);