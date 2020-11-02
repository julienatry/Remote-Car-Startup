#include <SoftwareSerial.h>

int warningLightsPin = 8;
int ignitionPin = 9;
int starterPin = 10;
char receivedData;

SoftwareSerial BTSerial(2, 3); // RX, TX pins for BT module

void setup() {
  pinMode(warningLightsPin, OUTPUT);
  pinMode(ignitionPin, OUTPUT);
  pinMode(starterPin, OUTPUT);

  Serial.begin(9600);
  BTSerial.begin(9600);
}

void loop() {
	if (BTSerial.available())
	{
		receivedData = BTSerial.read();
		Serial.write(receivedData);
	}
}