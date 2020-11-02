#include <SoftwareSerial.h>

int warningLightsPin = 8;
int ignitionPin = 9;
int starterPin = 10;

SoftwareSerial BTSerial(2, 3); // RX, TX pins for BT module

void setup() {
  pinMode(warningLightsPin, OUTPUT);
  pinMode(ignitionPin, OUTPUT);
  pinMode(starterPin, OUTPUT);

  BTSerial.begin();
}

void loop() {

}
