#include <stdlib.h>
#include <SoftwareSerial.h>

#define batteryVoltagePin A5
//#define isEngineRunningPin 6
#define accessoriesPin 8
#define ignitionPin 9
#define starterPin 10
//#define relay1 11
//#define relay2 12
//#define relay3 13

// Global variables
String receivedData;
int accessoriesState = 0;
int ignitionState = 0;
int starterState = 0;
int boostMode = 0;

// Battery check variables
float batteryVoltage_val, batteryVoltage_calc, batteryVoltage_vol, R1 = 103500.00, R2 = 10000.00; // R1 and R2 are values of the 2 resistors from the voltage divider

// Serial port (RX/TX) for bluetooth adapter
SoftwareSerial BTSerial(2, 3);

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  
  pinMode(accessoriesPin, OUTPUT);
  pinMode(ignitionPin, OUTPUT);
  pinMode(starterPin, OUTPUT);
  //pinMode(relay1, OUTPUT);
  //pinMode(relay2, OUTPUT);
  //pinMode(relay3, OUTPUT);
  pinMode(batteryVoltagePin, INPUT);

  Serial.println("RemoteCarStartup Ready");
}

void loop() {
  if (BTSerial.available())
  {
    receivedData = BTSerial.readStringUntil('\n'); // Or read byte by byte
    Serial.print("Received: ");
    Serial.println(receivedData);
  }

  if (receivedData == "EngineON")
  {
    Serial.println("Starting vehicle");
    delay(2000);
    startEngine();
  }else if (receivedData == "EngineOFF")
  {
    Serial.println("Stopping Engine");
    digitalWrite(ignitionPin, LOW);
    ignitionState = 0;
    digitalWrite(accessoriesPin, LOW);
    accessoriesState = 0;
  }else if (receivedData == "LowBoost")
  {
    Serial.println("Low Boost Mode");
    boostMode = 0;
  }else if (receivedData == "HighBoost")
  {
    Serial.println("High Boost Mode");
    boostMode = 1;
  }else if (receivedData == "AccessoriesOFF")
  {
    Serial.println("Accessories OFF");
    accessoriesState = 0;
    digitalWrite(accessoriesPin, LOW);
  }else if (receivedData == "AccessoriesON")
  {
    Serial.println("Accessories ON");
    accessoriesState = 1;
    digitalWrite(accessoriesPin, HIGH);
  }else if (receivedData == "IgnitionOFF")
  {
    Serial.println("Ignition OFF");
    ignitionState = 0;
    digitalWrite(ignitionPin, LOW);
  }else if (receivedData == "IgnitionON")
  {
    Serial.println("Ignition ON");
    ignitionState = 1;
    digitalWrite(ignitionPin, HIGH);
  }else if (receivedData == "StarterOFF")
  {
    Serial.println("Starter OFF");
    starterState = 0;
    digitalWrite(starterPin, LOW);
  }else if (receivedData == "StarterON")
  {
    Serial.println("Starter ON");
    starterState = 1;
    digitalWrite(starterPin, HIGH);
  }else if (receivedData == "Connected")
  {
    Serial.println("Connected to new device");
    BTSerial.print("BoostMode ");
    BTSerial.println(boostMode);
    BTSerial.print("AccessoriesState ");
    BTSerial.println(accessoriesState);
    BTSerial.print("IgnitionState ");
    BTSerial.println(ignitionState);
    BTSerial.print("StarterState ");
    BTSerial.println(starterState);
  }else if (receivedData == "BatteryVoltage")
  {
    batteryVoltage_val = analogRead(batteryVoltagePin);
    batteryVoltage_calc = (batteryVoltage_val * 5.00) / 1023.00;
    batteryVoltage_vol = batteryVoltage_calc / (R2/(R1+R2));
    BTSerial.print("Battery ");
    BTSerial.println(batteryVoltage_vol);
  }
  receivedData = "";
}



void startEngine() {
  digitalWrite(accessoriesPin, HIGH);
  accessoriesState = 1;
  digitalWrite(ignitionPin, HIGH);
  ignitionState = 1;

  // Delay after turning ignition ON, so the ECU can initialize properly
  delay(10000); // 10 sec

  digitalWrite(starterPin, HIGH);
  starterState = 1;

  //Used to detect if starter is still needed
  /*while (digitalRead(isEngineRunningPin) == LOW) {
    delay(200); // 200 ms
  }*/
  delay(5000);
  digitalWrite(starterPin, LOW);
  starterState = 0;
}
