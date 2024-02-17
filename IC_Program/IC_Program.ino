#include <stdlib.h>
#include <SoftwareSerial.h>

//#define batteryVoltagePin A5
#define isEngineRunningPin 6
#define accessoriesPin 8
#define ignitionPin 9
#define starterPin 10
#define angelEyesPin 11
#define lockPin 12
#define unlockPin 13
#define exhaustValveONPin 14
#define exhaustValveOFFPin 15

// Global variables
String receivedData;
char* angelEyesState = "500", lockState = "600", exhaustValveState = "700";
char currentChar;

// Serial port (RX/TX) for bluetooth adapter
SoftwareSerial BTSerial(2, 3);

void setup() {
  pinMode(accessoriesPin, OUTPUT);
  pinMode(ignitionPin, OUTPUT);
  pinMode(starterPin, OUTPUT);
  pinMode(angelEyesPin, OUTPUT);
  pinMode(lockPin, OUTPUT);
  pinMode(unlockPin, OUTPUT);
  pinMode(exhaustValveONPin, OUTPUT);
  pinMode(exhaustValveOFFPin, OUTPUT);
  //pinMode(batteryVoltagePin, INPUT);
  pinMode(isEngineRunningPin, INPUT);

  Serial.begin(9600);
  BTSerial.begin(9600);
}

void loop() {
  if (BTSerial.available())
  {
    currentChar = BTSerial.read();
    receivedData = receivedData + currentChar;

    // Detect message's end
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
    delay(2000);
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
    BTSerial.write(angelEyes("state"));
  }else if (receivedData == "EngineOff")
  {
    Serial.println("Stoping Engine");
    digitalWrite(ignitionPin, LOW);
    digitalWrite(accessoriesPin, LOW);
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
    BTSerial.write(carLock("state"));
  }else if (receivedData == "ExhaustOpen")
  {
    Serial.println("Opening exhaust valve");
    exhaustValve("ON");
  }else if (receivedData == "ExhaustClose")
  {
    Serial.println("Closing exhaust valve");
    exhaustValve("OFF");
  }else if (receivedData == "ExhaustState")
  {
    Serial.println("Sending Exhaust Valve State");
    BTSerial.write(exhaustValve("state"));
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
  digitalWrite(accessoriesPin, HIGH);
  digitalWrite(ignitionPin, HIGH);

  // Delay after turning ignition ON, so the ECU can initialize properly
  delay(10000); // 10 sec

  digitalWrite(starterPin, HIGH);

  //Used to detect if starter is still needed
  while (digitalRead(isEngineRunningPin) == LOW) {
    delay(200); // 200 ms
  }
  digitalWrite(starterPin, LOW);
}



char* carLock(String action) {
  if (action == "state")
  {
    return lockState;
  }else if (action == "lock")
  {
    digitalWrite(lockPin, HIGH);
    delay(200);
    digitalWrite(lockPin, LOW);
    lockState = "600";
  }else if (action == "unlock")
  {
    digitalWrite(unlockPin, HIGH);
    delay(200);
    digitalWrite(unlockPin, LOW);
    lockState = "601";
  }else{
    Serial.println("carLock : string error");
  }
}



char* exhaustValve(String action) {
  if (action == "state")
  {
    return exhaustValveState;
  }else if (action == "ON")
  {
    digitalWrite(exhaustValveONPin, HIGH);
    exhaustValveState = "701";
  }else if (action == "OFF")
  {
    digitalWrite(exhaustValveOFFPin, LOW);
    exhaustValveState = "700";
  }else{
    Serial.println("exhaustValve : string error");
  }
}