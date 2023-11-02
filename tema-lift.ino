#define START_LEVEL 0
#define END_LEVEL 2
#define DEBOUNCE_DELAY 30
#define TRAVEL_TIME 2500
#define BLINK_TIME 100
#define SOUND_TIME 700

const int blinkingLED = 12;
const int buttonPins[3] = {3, 4, 5};
const int LEDpins[3] = {9, 10, 11} ;
const int buzzerPin = 13;

unsigned int lastButtonStateLevel[3] = {0, 0, 0};
byte buttonStateLevel[3] = {LOW, LOW, LOW};
byte buttonPressedLevel[3] = {LOW, LOW, LOW};
unsigned int lastDebounceTimeLevel[3] = {0, 0, 0};
byte readingLevel[3] = {LOW, LOW, LOW};

unsigned int elevator = START_LEVEL;
unsigned int startTime = 0;
int state = 0;
unsigned long lastBlink = 0;
bool blinkState = 1;
int doors = 1;

void setup() {
  for(int level = START_LEVEL; level <= END_LEVEL ; level ++){
    pinMode(buttonPins[level], INPUT_PULLUP);
    pinMode(LEDpins[level], OUTPUT);
  }
  digitalWrite(LEDpins[START_LEVEL], HIGH);
  pinMode(blinkingLED, OUTPUT);
  tone(buzzerPin, 1000, SOUND_TIME);

}

void blink(){
  if(millis() > lastBlink + BLINK_TIME){
    blinkState = !blinkState;
    digitalWrite(blinkingLED, blinkState);
    lastBlink = millis();
  }
}

void loop() {

  digitalWrite(blinkingLED, blinkState);
   readingLevel[0] = digitalRead(buttonPins[0]);
  if (readingLevel[0] != lastButtonStateLevel[0]) {
    lastDebounceTimeLevel[0] = millis();
  }

  if ((millis() - lastDebounceTimeLevel[0]) > DEBOUNCE_DELAY) {
    if (readingLevel[0] != buttonStateLevel[0]) {
      buttonStateLevel[0] = readingLevel[0];
      if (buttonStateLevel[0] == LOW) {
          buttonPressedLevel[0] = 1;
      }
    }
  }
  lastButtonStateLevel[0] = readingLevel[0];

  
   readingLevel[1] = digitalRead(buttonPins[1]);
  if (readingLevel[1] != lastButtonStateLevel[1]) {
    lastDebounceTimeLevel[1] = millis();
  }

  if ((millis() - lastDebounceTimeLevel[1]) > DEBOUNCE_DELAY) {
    if (readingLevel[1] != buttonStateLevel[1]) {
      buttonStateLevel[1] = readingLevel[1];
      if (buttonStateLevel[1] == LOW) {
          buttonPressedLevel[1] = 1;
      }
    }
  }
  lastButtonStateLevel[1] = readingLevel[1];

  
   readingLevel[2] = digitalRead(buttonPins[2]);
  if (readingLevel[2] != lastButtonStateLevel[2]) {
    lastDebounceTimeLevel[2] = millis();
  }

  if ((millis() - lastDebounceTimeLevel[2]) > DEBOUNCE_DELAY) {
    if (readingLevel[2] != buttonStateLevel[2]) {
      buttonStateLevel[2] = readingLevel[2];
      if (buttonStateLevel[2] == LOW) {
          buttonPressedLevel[2] = 1;
      }
    }
  }
  lastButtonStateLevel[2] = readingLevel[2];

  //state 0 - the elevator is not moving
  //state 1 - the elevator is ascending
  //state -1 - the elevator is descending

  switch(state){

    case 0:

      if(buttonPressedLevel[elevator] == 1){
        buttonPressedLevel[elevator] = 0;
        tone(buzzerPin, 800, SOUND_TIME);
        doors=1;
        
      }

      for(int level = START_LEVEL ; level <= END_LEVEL; level++){
        if(buttonPressedLevel[level] == 1){
          startTime = millis();
          if( level > elevator ){
            state = 1;
          } 
          else {
            state = -1;
            
          }
          if(doors == 1){
            doors = 0;
            tone (buzzerPin, 1200, SOUND_TIME);
          }
          level = END_LEVEL;
        }
      }
      break;

    case 1: 
      blink();
      if(millis() >= startTime + TRAVEL_TIME){
        digitalWrite(LEDpins[elevator], LOW);
        elevator += 1;
        digitalWrite(LEDpins[elevator], HIGH);
        state = 0;
      }
    break;

    case -1:
      blink();
      if(millis() >= startTime + TRAVEL_TIME){
        digitalWrite(LEDpins[elevator], LOW);
        elevator -= 1;
        digitalWrite(LEDpins[elevator], HIGH);
        state = 0;
      }
    break;
  }


}
