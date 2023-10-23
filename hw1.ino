
int input0 = A0; // 1st potentiometer
int input1 = A1; // 2nd potentiometer
int input2 = A2; // 3rd potentiometer

int red_LED = 10;
int blue_LED = 11;  
int green_LED = 12;

int dark = 1023;
int bright = 0;
int led_max = 255;
int led_min = 0;

 // analogRead range from 0 to 1023, and an analogWrite range only from 0 to 255

void setup ()
{
  Serial.begin(9600); 
  pinMode(red_LED, OUTPUT); 
  pinMode(green_LED, OUTPUT);
  pinMode(blue_LED, OUTPUT);
}

void loop()
{
  int value0 = analogRead(input0); 
  int value1 = analogRead(input1); 
  int value2 = analogRead(input2); 

  int blue_value = map(value0, bright, dark, led_min, led_max); 
  int red_value = map(value1, bright, dark, led_min, led_max); 
  int green_value = map(value2, bright, dark, led_min, led_max); 

  analogWrite(red_LED, blue_value); 
  analogWrite(green_LED, red_value);
  analogWrite(blue_LED, green_value);
}
