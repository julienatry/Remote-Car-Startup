#include <SoftwareSerial.h>
#include "Adafruit_Soundboard.h"

int hornResetPin = 6, hornUGPin = 7;
int warningLightsPin = 8, ignitionPin = 9, starterPin = 10;
int angelEyesPin = 11, lockPin = 12, unlockPin = 13;																						//Pins

String receivedData;
char* angelEyesState = "500";
int hornTrack;

SoftwareSerial BTSerial(2, 3); 																												//RX, TX pins for BT module
SoftwareSerial HornSerial(4, 5);																											//RX, TX pins for custom horn

Adafruit_Soundboard horn(&HornSerial, NULL, hornResetPin);

void setup() {
  pinMode(warningLightsPin, OUTPUT);
  pinMode(ignitionPin, OUTPUT);
  pinMode(starterPin, OUTPUT);
  pinMode(angelEyesPin, OUTPUT);
  pinMode(hornUGPin, OUTPUT);
  pinMode(lockPin, OUTPUT);
  pinMode(unlockPin, OUTPUT);

  Serial.begin(9600);
  BTSerial.begin(9600);
}

void loop() {
	receivedData = ""; 																														//Reset

	if (BTSerial.available())
	{
		receivedData = BTSerial.readString();
	}

	if (receivedData == "StartupSequence")
	{
		Serial.println("Starting vehicle");
		unlockCar();
		startEngine();
	}else if (receivedData == "AngelEyesON")
	{
		Serial.println("Turning AngelEyes ON");
		angelEyes("ON");
	}else if (receivedData == "AngelEyesOFF")
	{
		Serial.println("Turning AngelEyes OFF");
		angelEyes("OFF");
	}else if (receivedData == "AreAngelEyesON")
	{
		BTSerial.write(angelEyes("state"));																									//Send 501 if AngelEyes ON or 500 if OFF
	}else if (receivedData == "EngineOff")
	{
		digitalWrite(ignitionPin, LOW);
	}else if (receivedData >= 800 && receivedData <= 811)
	{
		hornTrack = receivedData - 800;
		initHorn();
		playHorn(hornTrack);
	}else if (receivedData == "LockCar")
	{
		lockCar();
	}else if (receivedData == "UnlockCar")
	{
		unlockCar();
	}
}



char* angelEyes(String action) {
	if (action == "state")
	{
		return angelEyesState;
	}else if (action == "ON")
	{
		digitalWrite(angelEyesPin, HIGH);
		angelEyesState = "501";
	}else if (action == "OFF")
	{
		digitalWrite(angelEyesPin, LOW);
		angelEyesState = "500";
	}
}



void startEngine() {
	digitalWrite(ignitionPin, HIGH);																										//Turn ON ignition
	delay(10000);
	digitalWrite(starterPin, HIGH);																											//Turn ON starter for 5 sec
	delay(5000);
	digitalWrite(starterPin, LOW);
}



void warningLights(String act) {
	if (act == "lock")
	{
		digitalWrite(warningLightsPin, HIGH);
		delay(2000);
		digitalWrite(warningLightsPin, LOW);
	}else if (act == "unlock")
	{
		int count = 2;
		while(count > 0) {
			digitalWrite(warningLightsPin, HIGH);
			delay(500);
			digitalWrite(warningLightsPin, LOW);
			count = count - 1;
		}
	}else{
		Serial.println("warningLights : string error");
	}
}



void lockCar() {
	warningLights("lock");
	digitalWrite(lockPin, HIGH);
	delay(200);
	digitalWrite(lockPin, LOW);
}

void unlockCar() {
	warningLights("unlock");
	digitalWrite(unlockPin, HIGH);
	delay(200);
	digitalWrite(unlockPin, LOW);
}



void initHorn() {
	digitalWrite(hornUGPin, HIGH);
	delay(2000);
	HornSerial.begin(9600);

	if (!sfx.reset())
	{
		Serial.println("initHorn : Reset failed");
	}
}

void playHorn(int track) {																													//Play selected track on custom horn
	Serial.print("Playing track "); Serial.println(track);
	if (!sfx.playTrack(track))
	{
		Serial.println("playHorn : Failed to play track");
	}
}