#include <SoftwareSerial.h>

int warningLightsPin = 8;
int ignitionPin = 9;
int starterPin = 10;
int angelEyesPin = 11;
int hornPin = 12;
int areAngelEyesON = 0;
String receivedData;
char* state = "500";

SoftwareSerial BTSerial(2, 3); 											// RX, TX pins for BT module

void setup() {
  pinMode(warningLightsPin, OUTPUT);
  pinMode(ignitionPin, OUTPUT);
  pinMode(starterPin, OUTPUT);
  pinMode(angelEyesPin, OUTPUT);
  pinMode(hornPin, OUTPUT);

  Serial.begin(9600);
  BTSerial.begin(9600);
}

void loop() {
	receivedData = ""; 													// Reset

	if (BTSerial.available())
	{
		receivedData = BTSerial.readString();
	}

	if (receivedData == "StartupSequence")
	{
		Serial.print("Starting vehicle");
		startEngine();
	}else if (receivedData == "AngelEyesON")
	{
		Serial.print("Turning AngelEyes ON");
		angelEyes("ON");
	}else if (receivedData == "AngelEyesOFF")
	{
		Serial.print("Turning AngelEyes OFF");
		angelEyes("OFF");
	}else if (receivedData == "AreAngelEyesON")
	{
		BTSerial.write(angelEyes("state"));								//Send 501 if AngelEyes ON or 500 if OFF
	}
}

char* angelEyes(String action) {
	if (action == "state")
	{
		return state;
	}else if (action == "ON")
	{
		digitalWrite(angelEyesPin, HIGH);
		state = "501";
	}else if (action == "OFF")
	{
		digitalWrite(angelEyesPin, LOW);
		state = "500";
	}
}

void startEngine() {
	warningLights();
	digitalWrite(ignitionPin, HIGH);									//Turn ON ignition
	delay(10000);
	digitalWrite(starterPin, HIGH);										//Turn ON starter for sec
	delay(5000);
	digitalWrite(starterPin, LOW);
}

void warningLights() {
	digitalWrite(warningLightsPin, HIGH);
	delay(2000);
	digitalWrite(warningLightsPin, LOW);
}