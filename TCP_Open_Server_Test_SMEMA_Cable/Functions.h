// Functions.h

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Arduino.h>

// Function declarations
String displayLowPins(int value1a, int value1b, int value1c, int value1d,
                      int value2a, int value2b, int value2c, int value2d);
int parseDelayValue(String request);
void DeclarePins();
void calculateErrors(int value1a, int value1b, int value1c, int value1d,
                     int value2a, int value2b, int value2c, int value2d);

#endif
