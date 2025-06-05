#include <stdlib.h>
#include <SoftwareSerial.h>

//#define batteryVoltagePin A5
//#define isEngineRunningPin 6
#define accessoriesPin 8
#define ignitionPin 9
#define starterPin 10
//#define relay1 11
//#define relay2 12
//#define relay3 13

// Global variables
String receivedBuffer, receivedData;
char currentChar;

// Serial port (RX/TX) for bluetooth adapter
SoftwareSerial BTSerial(2, 3);

void setup() {
  pinMode(accessoriesPin, OUTPUT);
  pinMode(ignitionPin, OUTPUT);
  pinMode(starterPin, OUTPUT);
  //pinMode(relay1, OUTPUT);
  //pinMode(relay2, OUTPUT);
  //pinMode(relay3, OUTPUT);
  //pinMode(batteryVoltagePin, INPUT);

  Serial.begin(9600);
  BTSerial.begin(9600);
}

void loop() {
  if (BTSerial.available())
  {
    currentChar = BTSerial.read();
    receivedBuffer = receivedBuffer + currentChar;

    // Detect message's end
    if (currentChar == '\n')
    {
      receivedData = receivedBuffer.substring(0, receivedData.length() - 1);
      Serial.println(receivedData);
      receivedBuffer = "";
    }
  }

  if (receivedData == "StartupSequence")
  {
    Serial.println("Starting vehicle");
    delay(2000);
    startEngine();
  }else if (receivedData == "EngineOff")
  {
    Serial.println("Stopping Engine");
    digitalWrite(ignitionPin, LOW);
    digitalWrite(accessoriesPin, LOW);
  }
}



void startEngine() {
  digitalWrite(accessoriesPin, HIGH);
  digitalWrite(ignitionPin, HIGH);

  // Delay after turning ignition ON, so the ECU can initialize properly
  delay(10000); // 10 sec

  digitalWrite(starterPin, HIGH);

  //Used to detect if starter is still needed
  /*while (digitalRead(isEngineRunningPin) == LOW) {
    delay(200); // 200 ms
  }*/
  delay(5000);
  digitalWrite(starterPin, LOW);
}
