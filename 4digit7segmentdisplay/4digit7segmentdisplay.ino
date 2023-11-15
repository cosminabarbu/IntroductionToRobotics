
const int latchPin = 11;
const int clockPin = 10;
const int dataPin = 12; 

// Define connections to the digit control pins for a 4-digit display
const int segD1 = 4;
const int segD2 = 5;
const int segD3 = 6;
const int segD4 = 7;

const int lapButtonPin = 3;
const int startStopButtonPin = 2;
const int resetButtonPin = 9;
byte resetButtonRead = LOW;
byte startStopButtonState = LOW;
byte lapButtonState = HIGH;
byte lastLapButtonState = HIGH;
byte currentButtonState = HIGH;
bool countingActive = true; // assuming you want to start counting initially
bool countingStarted = false;
unsigned long lastLapButtonPressTime = 0;
bool lapButtonPressed = false;
int lapBuffer[4] = {0}; //stores the last 4 laps
int lapIndex = 0; // index for cycling through saved laps


int displayDigits[] = {segD1, segD2, segD3, segD4};
const int displayCount = 4; // Number of digits in the display

const int encodingsNumber = 16;// Define the number of unique encodings (0-9, A-F for hexadecimal)

byte byteEncodings[encodingsNumber] = { // Define byte encodings for the hexadecimal characters 0-F
//A B C D E F G DP 
  B11111100, // 0 
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110, // 9
  B11101110, // A
  B00111110, // b
  B10011100, // C
  B01111010, // d
  B10011110, // E
  B10001110  // F
};
// Variables for controlling the display update timing
unsigned long lastIncrement = 0;
unsigned long delayCount = 50; // Delay between updates (milliseconds)
unsigned long number = 0; // The number being displayed

const int debounceDelay = 50; 
unsigned long lastButtonPressTime = 0;
bool buttonPressed = false;

void setup() {
  // Initialize the pins connected to the shift register as outputs
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  // Initialize digit control pins and set them to LOW (off)
  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }  

  pinMode(resetButtonPin, INPUT_PULLUP); 
  pinMode(startStopButtonPin, INPUT_PULLUP);
  pinMode(lapButtonPin, INPUT_PULLUP);
  resetButtonRead = HIGH;

  // Begin serial communication for debugging purposes
  Serial.begin(9600);
}

void loop() {
  resetTimer();
  startStopButtonState = digitalRead(startStopButtonPin);
  lapButtonState = digitalRead(lapButtonPin);
  if(resetButtonRead == LOW){
    number = 0;
    countingStarted = false; // reset the countingStarted flag
  } else {
  if(startStopButtonState == LOW && currentButtonState == HIGH){ //button pressed

  if(millis() - lastButtonPressTime > debounceDelay){

     lastButtonPressTime = millis(); //update last button press time
     delayMicroseconds(500); //add a small delay to debounce the button

    if(!countingStarted){
      countingStarted = true;
      lastIncrement = millis();
    } else {
    countingActive = !countingActive; //toggle the counting state
    }
    
 buttonPressed = true;

  }
  }
  
  currentButtonState = startStopButtonState; //update the currentButtonState

  if(countingActive && countingStarted){

  if (millis() - lastIncrement > delayCount) {
    number++;
    number %= 10000; // wrap around after 9999
    lastIncrement = millis();
  }
  }

  }
   // check for lap button press
  if (lapButtonState == LOW && lastLapButtonState == HIGH) {
    
    if (countingActive) {
      // save lap time
      if (lapIndex < 4) {
        lapBuffer[lapIndex] = number;
        lapIndex++;
      } else {
        // if lap buffer is full, overwrite the oldest lap
        lapBuffer[0] = lapBuffer[1];
        lapBuffer[1] = lapBuffer[2];
        lapBuffer[2] = lapBuffer[3];
        lapBuffer[3] = number;
      }
      lapButtonPressed = true;
    } else {
      // cycle through saved laps when timer is paused
      number = lapBuffer[lapIndex % 4];
      lapIndex++;
    }

    lastLapButtonPressTime = millis();
  }


  lastLapButtonState = lapButtonState;

  //if counting has not started, display 0000

  if(!countingStarted){
    number = 0;
  }
  writeNumber(number);
} 

void writeReg(int digit) {
// clear all segments before shifting in new data
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, B00000000);
    digitalWrite(latchPin, HIGH);
    
    // prepare to shift data by setting the latch pin low
    digitalWrite(latchPin, LOW);
    // shift out the byte representing the current digit to the shift register
    shiftOut(dataPin, clockPin, MSBFIRST, digit);
    // latch the data onto the output pins by setting the latch pin high
    digitalWrite(latchPin, HIGH);
}

void activateDisplay(int displayNumber) {
  // turn off all digit control pins to avoid ghosting
  for (int i = 0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }
  // turn on the current digit control pin
  digitalWrite(displayDigits[displayNumber], LOW);
  

  // introduce a short delay to minimize ghosting effect
  delayMicroseconds(200);

}

void writeNumber(int number) {
  int currentNumber = number;
  int displayDigit = 3; // start with the least significant digit
  int lastDigit = 0;


  while (currentNumber != 0) {
    lastDigit = currentNumber % 10;
    activateDisplay(displayDigit);
    if(displayDigit == 2){
    writeReg(byteEncodings[lastDigit] | B00000001 );  
    } else {
    writeReg(byteEncodings[lastDigit] ); }
    delayMicroseconds(200); 
    displayDigit--;
    currentNumber /= 10;
    writeReg(B00000000); // clear the register to avoid ghosting
  }

  // fill the remaining unused digits with zero
  for (int i = displayDigit; i >= 0; i--) {
    activateDisplay(i);
    writeReg(byteEncodings[0] ); 
    // display zero
    delayMicroseconds(200);
    writeReg(B00000000);
  }
  }

  void resetTimer(){
    resetButtonRead = digitalRead(resetButtonPin);
  if (resetButtonRead == LOW ) { 
   number = 0;
   //clear all saved laps in the lapBuffer
   for(int i=0; i< 4; i++){
    lapBuffer[i] = 0;
   }
   lapIndex = 0; //reset the lap index
  }
  } 
