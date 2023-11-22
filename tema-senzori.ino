
#include <Arduino.h>
#include <EEPROM.h>

const int redLED = 5;
const int greenLED = 6;
const int blueLED = 7;

const int photocellPin = A0 ;
const int trigPin = 9 ;
const int echoPin = 10 ;

long duration = 0;
int distance = 0;

const int EEPROM_SIZE = 50;

// enum to represent different menus
enum Menu {
  MAIN_MENU,
  SENSOR_SETTINGS_MENU,
  RESET_LOGGER_DATA_MENU,
  SYSTEM_STATUS_MENU,
  RGB_LED_CONTROL_MENU
};

// global variable to store the current menu
Menu currentMenu = MAIN_MENU;

int sensorSamplingInterval = 3; //default value in seconds 
int ultrasonicValue = 10; 
int LDRValue = 400; 


void setup() {
  Serial.begin(9600);

  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);

  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
}

void loop() {

  ultrasonicValue = readUltrasonicSensor();
  LDRValue = readLDRSensor();

  switch (currentMenu) {
    case MAIN_MENU:
      showMainMenu();
      handleMainMenuInput(readUserInput());
      break;
    case SENSOR_SETTINGS_MENU:
      showSensorSettingsMenu();
      handleSensorSettingsInput(readUserInput());
      break;
    case RESET_LOGGER_DATA_MENU:
      showResetLoggerDataMenu();
      handleResetLoggerDataInput(readUserInput());
      break;
    case SYSTEM_STATUS_MENU:
      showSystemStatusMenu();
      handleSystemStatusInput(readUserInput());
      break;
    case RGB_LED_CONTROL_MENU:
      showRGBLEDControlMenu();
      handleRGBLEDControlInput(readUserInput());
      break;
    default:
      Serial.println("Invalid menu state.");
      break;
  }
}

void showMainMenu() {
  Serial.println("Main Menu:");
  Serial.println("1. Sensor Settings");
  Serial.println("2. Reset Logger Data");
  Serial.println("3. System Status");
  Serial.println("4. RGB LED Control");
  Serial.println("Enter your choice:");
}

void handleMainMenuInput(int choice) {
  switch (choice) {
    case 1:
      currentMenu = SENSOR_SETTINGS_MENU;
      break;
    case 2:
      currentMenu = RESET_LOGGER_DATA_MENU;
      break;
    case 3:
      currentMenu = SYSTEM_STATUS_MENU;
      break;
    case 4:
      currentMenu = RGB_LED_CONTROL_MENU;
      break;
    default:
      Serial.println("The selected option is outside the scope of the menu.");
      break;
  }
}

void showSensorSettingsMenu() {
  Serial.println("Sensor Settings Menu:");
  Serial.println("1. Sensors Sampling Interval");
  Serial.println("2. Ultrasonic Alert Threshold");
  Serial.println("3. LDR Alert Threshold");
  Serial.println("4. Back");
  Serial.println("Enter your choice:");
}

void handleSensorSettingsInput(int choice) {
  switch (choice) {
    case 1:
      Serial.println("Enter new sensors sampling interval (in seconds):");
      handleSensorSamplingInterval(readUserInput());
      break;
    case 2:
      Serial.println("Enter new ultrasonic alert threshold:");
      setUltrasonicAlert(readUserInput());
      break;
    case 3:
      Serial.println("Enter new LDR alert threshold:");
      setLDRAlert(readUserInput());
      break;
    case 4:
      currentMenu = MAIN_MENU; 
      break;
    default:
      Serial.println("The selected option is outside the scope of the menu.");
      break;
  }
}

void setUltrasonicAlert(int choice){
  if (choice < 2 or choice > 12){
    Serial.println("The value you introduced is outside the limits. Try again:");
    choice = readUserInput();
    setUltrasonicAlert(choice);
  }
  else {
    ultrasonicValue = choice;
    Serial.print("Ultrasonic sensor threshold value set to ");
    Serial.println(choice);
  }
}

void setLDRAlert(int choice){
  if (choice < 380 or choice > 550){
    Serial.println("The value you introduced is outside the limits. Try again:");
    choice = readUserInput();
    setLDRAlert(choice);
  }
  else {
    LDRValue = choice;
    Serial.print("LDR sensor threshold value set to ");
    Serial.println(choice);
  }
}

void handleSensorSamplingInterval(int choice){
  if(choice < 1 or choice > 10){
    Serial.println("The value you introduced is outside the limits. Try again:");
    choice = readUserInput(); 
    handleSensorSamplingInterval(choice); 
    }
  else 
  {
  sensorSamplingInterval = choice;
  Serial.print("Sensor sampling interval value set to ");
  Serial.print(sensorSamplingInterval);
  Serial.println(" seconds."); 
  }
}

void showResetLoggerDataMenu() {
  Serial.println("Reset Logger Data Menu:");
  Serial.println("1. Yes");
  Serial.println("2. No");
  Serial.println("Enter your choice:");
}

void handleResetLoggerDataInput(int choice) {
  switch (choice) {
    case 1:
      Serial.println("Logger data reset. Confirming action...");
      resetLoggerData();
      break;
    case 2:
      Serial.println("Data reset cancelled.");
      break;
    default:
      Serial.println("The selected option is outside the scope of the menu. ");
      break;
  }
  currentMenu = MAIN_MENU; 
}

void resetLoggerData(){
   for (int i = 0; i < EEPROM_SIZE; ++i) {
        EEPROM.update(i, 0);
      }
      sensorSamplingInterval = 3;
      ultrasonicValue = 10;
      LDRValue = 400;
}

void showSystemStatusMenu() {
  Serial.println("System Status Menu:");
  Serial.println("1. Current Sensor Readings");
  Serial.println("2. Current Sensor Settings");
  Serial.println("3. Display Logged Data");
  Serial.println("4. Back");
  Serial.println("Enter your choice:");
}

void handleSystemStatusInput(int choice) {
  switch (choice) {
    case 4:
      currentMenu = MAIN_MENU; 
      break;
    default:
      Serial.println("The selected option is outside the scope of the menu.");
      break;
  }
}

void showRGBLEDControlMenu() {
  Serial.println("RGB LED Control Menu:");
  Serial.println("1. Manual Color Control");
  Serial.println("2. LED: Toggle Automatic On/Off");
  Serial.println("3. Back");
  Serial.println("Enter your choice:");
}

void handleRGBLEDControlInput(int choice) {

  switch (choice) {
    
    case 1:
      Serial.println("Choose a color: ");
      Serial.println("1. Red");
      Serial.println("2. Green");
      Serial.println("3. Blue");
      chooseRGB(readUserInput());
      break;

    case 2:
      Serial.println("automatic");
      break;
      
    case 3:
      currentMenu = MAIN_MENU; 
      break;
  }

}

int readUserInput() {
  while (!Serial.available()) {
    // wait for user input
  }
  return Serial.parseInt();
}

void chooseRGB(int choice){

  switch(choice){
    case 1:
      digitalWrite(greenLED, LOW);
      digitalWrite(blueLED, LOW);
      digitalWrite(redLED, HIGH);
      break;
    case 2:
      digitalWrite(redLED, LOW);
      digitalWrite(blueLED, LOW);
      digitalWrite(greenLED, HIGH);
      break;
    case 3:
      digitalWrite(greenLED, LOW);
      digitalWrite(redLED, LOW);
      digitalWrite(blueLED, HIGH);
      break;
  }
  //  currentMenu = MAIN_MENU;
}

int readUltrasonicSensor(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  return distance;
}

int readLDRSensor() {

  return analogRead(photocellPin);
}
