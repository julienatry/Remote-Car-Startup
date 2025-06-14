#include <stdlib.h>
#include <SoftwareSerial.h>

#define batteryVoltagePin A5
//#define isEngineRunningPin 6
#define accessoriesPin 8
#define accessoriesPin2 9
#define ignitionPin 10
#define ignitionPin2 11
#define starterPin 12
#define starterPin2 13

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
  pinMode(accessoriesPin2, OUTPUT);
  pinMode(ignitionPin, OUTPUT);
  pinMode(ignitionPin2, OUTPUT);
  pinMode(starterPin, OUTPUT);
  pinMode(starterPin2, OUTPUT);
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
    digitalWrite(ignitionPin2, LOW);
    ignitionState = 0;
    digitalWrite(accessoriesPin, LOW);
    digitalWrite(accessoriesPin2, LOW);
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
    digitalWrite(accessoriesPin2, LOW);
  }else if (receivedData == "AccessoriesON")
  {
    Serial.println("Accessories ON");
    accessoriesState = 1;
    digitalWrite(accessoriesPin, HIGH);
    digitalWrite(accessoriesPin2, HIGH);
  }else if (receivedData == "IgnitionOFF")
  {
    Serial.println("Ignition OFF");
    ignitionState = 0;
    digitalWrite(ignitionPin, LOW);
    digitalWrite(ignitionPin2, LOW);
  }else if (receivedData == "IgnitionON")
  {
    Serial.println("Ignition ON");
    ignitionState = 1;
    digitalWrite(ignitionPin, HIGH);
    digitalWrite(ignitionPin2, HIGH);
  }else if (receivedData == "StarterOFF")
  {
    Serial.println("Starter OFF");
    starterState = 0;
    digitalWrite(starterPin, LOW);
    digitalWrite(starterPin2, LOW);
  }else if (receivedData.startsWith("StarterON"))
  {
    int spaceIndex = receivedData.indexOf(' ');
    int starterDuration = 5;

    if (spaceIndex != -1) {
      String durationString = receivedData.substring(spaceIndex + 1);
      int parsedDuration = durationString.toInt();
      if (parsedDuration > 0 && parsedDuration <= 10) {
        starterDuration = parsedDuration;
      }
    }
    
    Serial.print("Starter ON for ");
    Serial.print(starterDuration);
    Serial.println(" seconds");
    
    starterState = 1;
    digitalWrite(starterPin, HIGH);
    digitalWrite(starterPin2, HIGH);
    BTSerial.print("StarterState ");
    BTSerial.println(starterState);

    delay(starterDuration * 1000);

    digitalWrite(starterPin, LOW);
    digitalWrite(starterPin2, LOW);
    starterState = 0;
    Serial.println("Starter OFF");
    BTSerial.print("StarterState ");
    BTSerial.println(starterState);
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
  digitalWrite(accessoriesPin2, HIGH);
  accessoriesState = 1;
  digitalWrite(ignitionPin, HIGH);
  digitalWrite(ignitionPin2, HIGH);
  ignitionState = 1;

  // Delay after turning ignition ON, so the ECU can initialize properly
  /*delay(10000); // 10 sec

  digitalWrite(starterPin, HIGH);
  digitalWrite(starterPin2, HIGH);
  starterState = 1;

  //Used to detect if starter is still needed
  /*while (digitalRead(isEngineRunningPin) == LOW) {
    delay(200); // 200 ms
  }
  delay(5000);
  digitalWrite(starterPin, LOW);
  digitalWrite(starterPin2, LOW);
  starterState = 0;*/
}
