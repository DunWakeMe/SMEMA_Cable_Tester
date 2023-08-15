#include <Ethernet.h>
#include <SPI.h>
#include <Controllino.h>
#include "ConstantsAndVariables.h"
#include "Functions.h"

/* This code allows Arduino monitor certain pins,
   keep track of low signal events,
   and communicate with other devices over TCP.

   Type 192.168.1.177:5000 to access the checking progress in a browser URL.

   Type 192.168.1.177:5000/?delay=100 or 192.168.1.177:5000/?delay=5000 or the number between 100 and 5000 to
   change the checking time. 100 being 100ms and 5000 being 5 seconds.
*/

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);
EthernetServer server(5000);

const int Dout1a = 3;
const int Dout1b = 5;
const int Dout1c = 7;
const int Dout1d = 9;

const int Dout2a = 11;
const int Dout2b = 13;
const int Dout2c = 43;
const int Dout2d = 45;

const int Ain1a = A1;
const int Ain1b = A3;
const int Ain1c = A5;
const int Ain1d = A7;

const int Ain2a = A9;
const int Ain2b = A11;
const int Ain2c = A13;
const int Ain2d = A15;

unsigned long counter = 0;
unsigned long errorCount[8] = {0}; // Array to store error count for each pin
unsigned long totalErrorCount = 0; // Variable to store the total error count
int delayTime = 500;

void setup() {
  DeclarePins();

  Ethernet.begin(mac, ip);
  server.begin();
  Serial.begin(9600);
}

void loop() {
  counter++;

  digitalWrite(Dout1a, HIGH);
  digitalWrite(Dout1b, HIGH);
  digitalWrite(Dout1c, HIGH);
  digitalWrite(Dout1d, HIGH);
  digitalWrite(Dout2a, HIGH);
  digitalWrite(Dout2b, HIGH);
  digitalWrite(Dout2c, HIGH);
  digitalWrite(Dout2d, HIGH);

  int value1a = digitalRead(Ain1a);
  int value1b = digitalRead(Ain1b);
  int value1c = digitalRead(Ain1c);
  int value1d = digitalRead(Ain1d);
  int value2a = digitalRead(Ain2a);
  int value2b = digitalRead(Ain2b);
  int value2c = digitalRead(Ain2c);
  int value2d = digitalRead(Ain2d);

  calculateErrors(value1a, value1b, value1c, value1d, value2a, value2b, value2c, value2d);

  delay(delayTime);

  digitalWrite(Dout1a, LOW);
  digitalWrite(Dout1b, LOW);
  digitalWrite(Dout1c, LOW);
  digitalWrite(Dout1d, LOW);
  digitalWrite(Dout2a, LOW);
  digitalWrite(Dout2b, LOW);
  digitalWrite(Dout2c, LOW);
  digitalWrite(Dout2d, LOW);

  delay(delayTime);

  EthernetClient client = server.available();
  if (client) {
    Serial.println("New client");
    boolean currentLineIsBlank = true;
    char line[80];
    int index = 0;

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);

        line[index] = c;
        index++;

        if (c == '\n' && currentLineIsBlank) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println("Refresh: 5");
          client.println();

          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head>");
          client.println("<style>");
          client.println("body { font-family: Arial, sans-serif; }");
          client.println("h2 { color: #333; }");
          client.println("ul { list-style: none; padding: 0; }");
          client.println("li { margin-bottom: 5px; }");
          client.println("</style>");
          client.println("</head>");
          client.println("<body>");

          if (counter % 1 == 0) {
            client.println("<h2>Loop count: " + String(counter) + "</h2>");

            // Display the total error count
            client.println("<h2>Total Error count: " + String(totalErrorCount) + "</h2>");

            // Display the error count for each pin
            client.println("<h2>Error counts for each pin:</h2>");
            client.println("<ul>");
            client.println("<li>Error count for A1in1a: " + String(errorCount[0]) + "</li>");
            client.println("<li>Error count for A3in1b: " + String(errorCount[1]) + "</li>");
            client.println("<li>Error count for A5in1c: " + String(errorCount[2]) + "</li>");
            client.println("<li>Error count for A7in1d: " + String(errorCount[3]) + "</li>");
            client.println("<li>Error count for A9in2a: " + String(errorCount[4]) + "</li>");
            client.println("<li>Error count for A11in2b: " + String(errorCount[5]) + "</li>");
            client.println("<li>Error count for A13in2c: " + String(errorCount[6]) + "</li>");
            client.println("<li>Error count for A15in2d: " + String(errorCount[7]) + "</li>");
            client.println("</ul>");

            // Display the low pins in the response
            client.print(displayLowPins(value1a, value1b, value1c, value1d,
                                        value2a, value2b, value2c, value2d));

          }

          client.println("</body>");
          client.println("</html>");
          break;
        }

        if (c == '\n') {
          line[index] = 0;
          if (strstr(line, "GET /?delay=") == line) {
            int newDelayTime = parseDelayValue(line);

            if (newDelayTime != -1) {
              delayTime = newDelayTime;
            } else {
              client.println("HTTP/1.1 400 Bad Request");
              client.println("Content-Type: text/plain");
              client.println("Connection: close");
              client.println();
              client.println("Invalid delay value. Please provide an integer between 100(ms) to 5000(ms).");
              client.println();
              break;
            }
          }
          currentLineIsBlank = true;
          index = 0;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
