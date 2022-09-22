#pragma once

#include <Arduino.h>

void setup_pwmRead();
boolean RC_avail();
void print_RCpwm();
int RC_decode(int CH);