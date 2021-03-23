#include <stdlib.h>
#include <SoftwareSerial.h>

int batteryVoltagePin = A5, val = 0;
int accessoriesPin = 8, ignitionPin = 9, starterPin = 10;
int angelEyesPin = 11, lockPin = 12, unlockPin = 13;
String receivedData;
char* angelEyesState = "500", lockState = "600";
char currentChar;

float calc = 0.00;
float voltageBattery = 0.00;
float R1 = 1011000.00;
float R2 = 474000.00;

int i = 0;

SoftwareSerial BTSerial(2, 3);

void setup() {
  pinMode(accessoriesPin, OUTPUT);
  pinMode(ignitionPin, OUTPUT);
  pinMode(starterPin, OUTPUT);
  pinMode(angelEyesPin, OUTPUT);
  pinMode(lockPin, OUTPUT);
  pinMode(unlockPin, OUTPUT);
  pinMode(batteryVoltagePin, INPUT);

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
  
  
  if (i > 25000)
  {
    i = 0;
    Serial.println("Sending battery voltage");
    BTSerial.print(batteryVoltage());
    BTSerial.println("V");
  }
  i++;
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
  delay(10000);
  digitalWrite(starterPin, HIGH);
  delay(5000);
  digitalWrite(starterPin, LOW);
}



/*void warningLights(String act) {
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
}*/



char* carLock(String action) {
  if (action == "state")
  {
    return lockState;
  }else if (action == "lock")
  {
    //warningLights("lock");
    digitalWrite(lockPin, HIGH);
    delay(200);
    digitalWrite(lockPin, LOW);
    lockState = "600";
  }else if (action == "unlock")
  {
    //warningLights("unlock");
    digitalWrite(unlockPin, HIGH);
    delay(200);
    digitalWrite(unlockPin, LOW);
    lockState = "601";
  }else{
    Serial.println("carLock : string error");
  }
}

float batteryVoltage() {
  val = analogRead(batteryVoltagePin);
  calc = (val * 5.00) / 1024.00;
  voltageBattery = calc / (R2/(R1+R2));
  Serial.println(voltageBattery);
  return voltageBattery;
}