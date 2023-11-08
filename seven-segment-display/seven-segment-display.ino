const int pinA = 12;
const int pinB = 10;
const int pinC = 9;
const int pinD = 8;
const int pinE = 7;
const int pinF = 6;
const int pinG = 5;
const int pinDP = 4;

const int pinSW = 2; // digital pin connected to switch output
const int pinX = A0; // A0 - analog pin connected to X output
const int pinY = A1; // A1 - analog pin connected to Y output

byte segState = LOW;
byte dpState = LOW;
byte swState = LOW;


bool commonAnode = false;
const int segSize = 8;
int index = 0;
int segments[segSize] = {pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP};
int lastSegState[segSize] = {
  segState, segState, segState, segState, segState, segState, segState, segState};
int xValue = 0;
int yValue = 0;

unsigned long lastBlink = 0;
bool blinkState = 1;
const int blinkTime = 500;
int currentPosition = segSize - 1;
int lastPosition = segSize - 1;

const int a = 0;
const int b = 1;
const int c = 2;
const int d = 3;
const int e = 4;
const int f = 5;
const int g = 6;
const int dp = 7;
const int NA = -1;

unsigned long buttonPressTime = 0;
unsigned long lastButtonReleaseTime = 0;
const int longPressDuration = 1000; // Define the duration for a long press (in milliseconds)


enum ButtonState {IDLE, SHORT_PRESS, LONG_PRESS};
ButtonState currentState = IDLE;

const int minTreshold = 400;
const int maxTreshold = 600;
bool joyMoved = false;

const int directions = 4; // up down left right
const int up = 0;
const int down = 1;
const int left = 2;
const int right = 3;
int scope = 0;

unsigned int movements[segSize][directions] = {
  // pe fiecare rand se regasesc valorile pinilor de la segmentele care sunt in directia precizata 
  //in comentariul de mai jos sau 0 in cazul in care miscarea intr-o anumita directie nu este posibila
  //up down left right 
   {NA, g,  f,  b}, // a 
   { a, g,  f, NA}, // b 
   { g, d,  e, dp}, // c 
   { g, NA, e,  c}, // d 
   { g, d,  NA, c}, // e 
   { a, g,  NA, b}, // f  
   { a, d, NA, NA}, // g 
   {NA, NA, c, NA}  // dp 
   };

void setup() {
  
  pinMode(pinSW, INPUT_PULLUP);

   if (commonAnode == true) {
    segState = !segState;
  }
    for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }
 
Serial.begin(9600);
}

void blink(){
  if(millis() > lastBlink + blinkTime){
    blinkState = !blinkState;
    digitalWrite(segments[currentPosition], blinkState);
    lastBlink = millis();
  }
}


void loop() {
  
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  swState = digitalRead(pinSW);

  blink(); 

  if((xValue >= minTreshold && xValue <= maxTreshold) and (yValue >= minTreshold && yValue <= maxTreshold)){
    joyMoved = false;
  }

  if(yValue < minTreshold && joyMoved == false){//we can go up
     move(up);
     joyMoved = true;
  }

  if(yValue > maxTreshold && joyMoved == false){//we can go downn
    move(down);
    joyMoved = true;
  }

  if(xValue < minTreshold && joyMoved == false ){ //we can go left
    move(left);
    joyMoved = true;
  }

  if(xValue > maxTreshold && joyMoved == false){//we can go right
    move(right);
    joyMoved = true;
  }

  if (swState == LOW) {
    segState = !segState;  // Toggle the LED state

    // Update the LED state for the current segment
    digitalWrite(segments[currentPosition], segState);

    // Remember the state for the next button press
    lastSegState[currentPosition] = segState;

    delay(200);  // Add a small delay to avoid rapid button presses
  }

    // State machine to handle button press
  switch (currentState) {
    case IDLE:
      if (swState == LOW) {
        buttonPressTime = millis();
        currentState = SHORT_PRESS;
      }
      break;

    case SHORT_PRESS:
      if (swState == HIGH) {
        if (millis() - buttonPressTime >= longPressDuration) {
          currentState = LONG_PRESS;
        } else {
          // Handle short press action here
          segState = !segState;
          digitalWrite(segments[currentPosition], segState);
        }
        currentState = IDLE;
      }
      break;

    case LONG_PRESS:
      if (swState == HIGH) {
        // Handle long press action here (calling reset function)
        reset();
        currentState = IDLE;
      }
      break;
  }
}

void move(int way){
scope = movements[currentPosition][way];
if(scope != -1){
  lastPosition = currentPosition;
  currentPosition = scope;
  digitalWrite(segments[lastPosition], lastSegState[lastPosition]);
}
}

void reset() {
  for (int i = 0; i < segSize; i++) {
    lastSegState[i] = LOW;
  }
  currentPosition = segSize - 1;
}