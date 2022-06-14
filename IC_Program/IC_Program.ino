#include <stdlib.h>
#include <SoftwareSerial.h>

#define batteryVoltagePin A5
#define accessoriesPin 8
#define ignitionPin 9
#define starterPin 10
#define angelEyesPin 11
#define lockPin 12
#define unlockPin 13
//#define isEngineRunningPin 

// Global variables
String receivedData;
char* angelEyesState = "500", lockState = "600";
char currentChar;

// Battery check variables
const long checkBatteryVoltage_interval = 60000; // 60 sec
unsigned long checkBatteryVoltage_lastMillis;
float calc = 0.00, voltageBattery = 0.00, R1 = 102000.00, R2 = 9999.00; // R1 and R2 are values of the 2 resistors from the voltage divider

// Serial port (RX/TX) for bluetooth adapter
SoftwareSerial BTSerial(2, 3);

void setup() {
  pinMode(accessoriesPin, OUTPUT);
  pinMode(ignitionPin, OUTPUT);
  pinMode(starterPin, OUTPUT);
  pinMode(angelEyesPin, OUTPUT);
  pinMode(lockPin, OUTPUT);
  pinMode(unlockPin, OUTPUT);
  pinMode(batteryVoltagePin, INPUT);
  //pinMode(isEngineRunningPin, INPUT);

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
  }
  
  checkBatteryVoltage();
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



void checkBatteryVoltage() {
  unsigned long checkBatteryVoltage_currentMillis = millis();

  if (checkBatteryVoltage_lastMillis > checkBatteryVoltage_currentMillis)
  {
    checkBatteryVoltage_lastMillis = checkBatteryVoltage_currentMillis;
  }

  if (checkBatteryVoltage_currentMillis - checkBatteryVoltage_lastMillis >= checkBatteryVoltage_interval)
  {
    checkBatteryVoltage_lastMillis = checkBatteryVoltage_currentMillis;

    checkBatteryVoltage_val = analogRead(batteryVoltagePin);
    checkBatteryVoltage_calc = (checkBatteryVoltage_val * 5.05) / 1023.00; // 5V is the IC's input voltage and 1023 is the max value the analogRead can return (1024 values including 0)
    checkBatteryVoltage_batteryVoltage = checkBatteryVoltage_calc / (R2/(R1+R2));
    Serial.println(checkBatteryVoltage_batteryVoltage);
  }

  if (checkBatteryVoltage_batteryVoltage < 12.3)
  {
    lowBatteryIdle();
  }
}

void lowBatteryIdle() {
  while(checkBatteryVoltage_batteryVoltage < 12.3) {
    delay(3600000); // 1 hour
    checkBatteryVoltage();
  }
}