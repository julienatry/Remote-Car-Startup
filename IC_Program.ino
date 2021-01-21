#include <stdlib.h>
#include <SoftwareSerial.h>

int warningLightsPin = 8, ignitionPin = 9, starterPin = 10;
int angelEyesPin = 11, lockPin = 12, unlockPin = 13;														//Pins

String receivedData;
char* angelEyesState = "500", lockState = "600";
char currentChar;

SoftwareSerial BTSerial(2, 3); 																				//RX, TX pins for BT module

void setup() {
  pinMode(warningLightsPin, OUTPUT);
  pinMode(ignitionPin, OUTPUT);
  pinMode(starterPin, OUTPUT);
  pinMode(angelEyesPin, OUTPUT);
  pinMode(lockPin, OUTPUT);
  pinMode(unlockPin, OUTPUT);

  Serial.begin(9600);
  BTSerial.begin(9600);
}

void loop() {
	if (BTSerial.available())
	{
		currentChar = BTSerial.read();
		receivedData = receivedData + currentChar;

		if (currentChar == '\n')
		{
			receivedData = receivedData.substring(0, receivedData.length() - 1);
			Serial.println(receivedData);
			receivedData = "";
		}
	}

	if (receivedData == "StartupSequence")
	{
		Serial.println("Starting vehicle");
		carLock("unlock");
		startEngine();
	}else if (receivedData == "AngelEyesOn")
	{
		Serial.println("Turning AngelEyes ON");
		angelEyes("ON");
	}else if (receivedData == "AngelEyesOff")
	{
		Serial.println("Turning AngelEyes OFF");
		angelEyes("OFF");
	}else if (receivedData == "AngelEyesState")
	{
		Serial.println("Sending Angel Eyes State");
		BTSerial.write(angelEyes("state"));																	//Send 501 if AngelEyes ON or 500 if OFF
	}else if (receivedData == "EngineOff")
	{
		Serial.println("Stoping Engine");
		digitalWrite(ignitionPin, LOW);
	}else if (receivedData == "LockCar")
	{
		Serial.println("Locking car");
		carLock("lock");
	}else if (receivedData == "UnlockCar")
	{
		Serial.println("Unlocking car");
		carLock("unlock");
	}else if (receivedData == "LockState")
	{
		Serial.println("Sending Car Lock State");
		BTSerial.write(carLock("state"));																	//Send 601 if the car is locked or 600 if the car is unlocked
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
	}else{
		Serial.println("angelEyes : string error");
	}
}



void startEngine() {
	digitalWrite(ignitionPin, HIGH);																		//Turn ON ignition
	delay(10000);
	digitalWrite(starterPin, HIGH);																			//Turn ON starter for 5 sec
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



char* carLock(String action) {
	if (action == "state")
	{
		return lockState;
	}else if (action == "lock")
	{
		warningLights("lock");
		digitalWrite(lockPin, HIGH);
		delay(200);
		digitalWrite(lockPin, LOW);
		lockState = "600";
	}else if (action == "unlock")
	{
		warningLights("unlock");
		digitalWrite(unlockPin, HIGH);
		delay(200);
		digitalWrite(unlockPin, LOW);
		lockState = "601";
	}else{
		Serial.println("carLock : string error");
	}
}