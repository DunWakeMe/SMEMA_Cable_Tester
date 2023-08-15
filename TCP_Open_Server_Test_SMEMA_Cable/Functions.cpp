// Functions.cpp

#include <Arduino.h>
#include "Functions.h"
#include "ConstantsAndVariables.h"

// Function definitions

String displayLowPins(int value1a, int value1b, int value1c, int value1d,
                      int value2a, int value2b, int value2c, int value2d) {
  String result = "<h2>Current Pin(s) Error(s):</h2><ul>";

  if (value1a == LOW) {
    result += "<li>" + String(millis() / 1000) + " - Error in Ain1a</li>";
    errorCount[0]++;
  }
  if (value1b == LOW) {
    result += "<li>" + String(millis() / 1000) + " - Error in Ain1b</li>";
    errorCount[1]++;
  }
  if (value1c == LOW) {
    result += "<li>" + String(millis() / 1000) + " - Error in Ain1c</li>";
    errorCount[2]++;
  }
  if (value1d == LOW) {
    result += "<li>" + String(millis() / 1000) + " - Error in Ain1d</li>";
    errorCount[3]++;
  }
  if (value2a == LOW) {
    result += "<li>" + String(millis() / 1000) + " - Error in Ain2a</li>";
    errorCount[4]++;
  }
  if (value2b == LOW) {
    result += "<li>" + String(millis() / 1000) + " - Error in Ain2b</li>";
    errorCount[5]++;
  }
  if (value2c == LOW) {
    result += "<li>" + String(millis() / 1000) + " - Error in Ain2c</li>";
    errorCount[6]++;
  }
  if (value2d == LOW) {
    result += "<li>" + String(millis() / 1000) + " - Error in Ain2d</li>";
    errorCount[7]++;
  }

  result += "</ul>";
  return result;
}

int parseDelayValue(String request) {
  int delayValue = -1; // Default value indicating an invalid input
  int delayStartIndex = request.indexOf("delay=");
  if (delayStartIndex != -1) {
    int delayEndIndex = request.indexOf(" ", delayStartIndex);
    if (delayEndIndex == -1) {
      delayEndIndex = request.length();
    }
    String delayString = request.substring(delayStartIndex + 6, delayEndIndex);

    // Check for decimal points in the delay string
    if (delayString.indexOf('.') == -1) {
      // No decimal point found, attempt to convert to an integer
      int intValue = delayString.toInt();
      if (intValue >= 100 && intValue <= 5000) {
        delayValue = intValue;
      }
    }
  }
  return delayValue;
}

void DeclarePins() {
  pinMode(Dout1a, OUTPUT);
  pinMode(Dout1b, OUTPUT);
  pinMode(Dout1c, OUTPUT);
  pinMode(Dout1d, OUTPUT);
  pinMode(Dout2a, OUTPUT);
  pinMode(Dout2b, OUTPUT);
  pinMode(Dout2c, OUTPUT);
  pinMode(Dout2d, OUTPUT);

  pinMode(Ain1a, INPUT);
  pinMode(Ain1b, INPUT);
  pinMode(Ain1c, INPUT);
  pinMode(Ain1d, INPUT);
  pinMode(Ain2a, INPUT);
  pinMode(Ain2b, INPUT);
  pinMode(Ain2c, INPUT);
  pinMode(Ain2d, INPUT);
}

void calculateErrors(int value1a, int value1b, int value1c, int value1d, int value2a, int value2b, int value2c, int value2d) {
  if (value1a == LOW || value1b == LOW || value1c == LOW || value1d == LOW ||
      value2a == LOW || value2b == LOW || value2c == LOW || value2d == LOW) {
    errorCount[0] += (value1a == LOW);
    errorCount[1] += (value1b == LOW);
    errorCount[2] += (value1c == LOW);
    errorCount[3] += (value1d == LOW);
    errorCount[4] += (value2a == LOW);
    errorCount[5] += (value2b == LOW);
    errorCount[6] += (value2c == LOW);
    errorCount[7] += (value2d == LOW);
  }

  totalErrorCount = 0; // Reset total error count before calculating
  for (int i = 0; i < 8; i++) {
    totalErrorCount += errorCount[i]; // Calculate total error count
  }
}
