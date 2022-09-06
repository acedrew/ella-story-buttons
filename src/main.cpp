#include <Arduino.h>

#define RED_LED 32
#define GREEN_LED 25
#define BLUE_LED 33
#define YELLOW_LED 26

#define RED_CHANNEL 0
#define GREEN_CHANNEL 1
#define BLUE_CHANNEL 2
#define YELLOW_CHANNEL 3


#define RED_BUTTON 36
#define GREEN_BUTTON 34
#define BLUE_BUTTON 39
#define YELLOW_BUTTON 35


#define DEBOUNCE_DELAY 50

uint32_t red_buffer = 0;
uint32_t green_buffer = 0;
uint32_t blue_buffer = 0;
uint32_t yellow_buffer = 0;

bool red_state = false;
bool green_state = false;
bool blue_state = false;
bool yellow_state = false;

bool red_fired = false;
bool green_fired = false;
bool blue_fired = false;
bool yellow_fired = false;

uint8_t led_mode = 0;
uint32_t led_pattern = 0;
uint8_t sineTable[255] = {0};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);

  pinMode(RED_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_BUTTON, INPUT_PULLUP);
  pinMode(BLUE_BUTTON, INPUT_PULLUP);
  pinMode(YELLOW_BUTTON, INPUT_PULLUP);

  ledcAttachPin(RED_LED, 0);
  ledcAttachPin(GREEN_LED, 1);
  ledcAttachPin(BLUE_LED, 2);
  ledcAttachPin(YELLOW_LED, 3);
  ledcSetup(RED_CHANNEL, 32000, 8);
  ledcSetup(GREEN_CHANNEL, 32000, 8);
  ledcSetup(BLUE_CHANNEL, 32000, 8);
  ledcSetup(YELLOW_CHANNEL, 32000, 8);
  for (uint i=0; i<255; i++) {
    sineTable[i] = 128 + (uint8_t)(((sin(((2.0*PI)/255)*i))*128)); 
    // Serial.println(sineTable[i]);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!digitalRead(RED_BUTTON)){
    if (!red_state) {
      red_buffer = millis();
      led_mode = 1;
    } else {
      if (millis() - red_buffer > DEBOUNCE_DELAY && !red_fired) {
        Serial.println("R");
        red_buffer = millis();
        red_fired = true;
      }
    }
    red_state = true;
    green_state = false;
    blue_state = false;
    yellow_state = false;

  } else if(red_state) {
    red_state = false;
    red_fired = false;
    led_mode = 0;
  }

  if (!digitalRead(GREEN_BUTTON)){
    if (!green_state) {
      green_buffer = millis();
      led_mode = 2;
    } else {
      if (millis() - green_buffer > DEBOUNCE_DELAY && !green_fired) {
        Serial.println("G");
        green_buffer = millis();
        green_fired = true;
      }
    }
    green_state = true;
    blue_state = false;
    yellow_state = false;
    red_state = false;

  } else if(green_state) {
    green_state = false;
    green_fired = false;
    led_mode = 0;
  }

  if (!digitalRead(BLUE_BUTTON)){
    if (!blue_state) {
      blue_buffer = millis();
      led_mode = 3;
    } else {
      if (millis() - blue_buffer > DEBOUNCE_DELAY && !blue_fired) {
        Serial.println("B");
        blue_buffer = millis();
        blue_fired = true;
      }
    }
    blue_state = true;
    yellow_state = false;
    green_state = false;
    red_state = false;

  } else if (blue_state) {
    blue_state = false;
    blue_fired = false;
    led_mode = 0;
  }

  if (!digitalRead(YELLOW_BUTTON)){
    if (!yellow_state) {
      yellow_buffer = millis();
      led_mode = 4;
    } else {
      if (millis() - yellow_buffer > DEBOUNCE_DELAY && !yellow_fired) {
        Serial.println("Y");
        yellow_buffer = millis();
        yellow_fired = true;
      }
    }
    yellow_state = true;
    green_state = false;
    blue_state = false;
    red_state = false;

  } else if (yellow_state) {
    yellow_state = false;
    yellow_fired = false;
    led_mode = 0;
  }
  switch(led_mode) {
    case 0:
      ledcWrite(RED_CHANNEL, sineTable[(led_pattern/200)%255]);
      ledcWrite(GREEN_CHANNEL, sineTable[(led_pattern/200 + 128)%255]);
      ledcWrite(BLUE_CHANNEL, sineTable[((led_pattern/200)+64)%255]);
      ledcWrite(YELLOW_CHANNEL, sineTable[((led_pattern/200)+192)%255]);
      break;
    case 1:
      ledcWrite(RED_CHANNEL, sineTable[(led_pattern/50)%255]);
      ledcWrite(GREEN_CHANNEL, 0);
      ledcWrite(BLUE_CHANNEL, 0);
      ledcWrite(YELLOW_CHANNEL, 0);
      break;
    case 2:
      ledcWrite(RED_CHANNEL, 0);
      ledcWrite(GREEN_CHANNEL, sineTable[(led_pattern/50 + 128)%255]);
      ledcWrite(BLUE_CHANNEL, 0);
      ledcWrite(YELLOW_CHANNEL, 0);
      break;
    case 3:
      ledcWrite(RED_CHANNEL, 0);
      ledcWrite(GREEN_CHANNEL, 0);
      ledcWrite(BLUE_CHANNEL, sineTable[((led_pattern/50)+64)%255]);
      ledcWrite(YELLOW_CHANNEL, 0);
      break;
    case 4:
      ledcWrite(RED_CHANNEL, 0);
      ledcWrite(GREEN_CHANNEL, 0);
      ledcWrite(BLUE_CHANNEL, 0);
      ledcWrite(YELLOW_CHANNEL, sineTable[((led_pattern/50)+192)%255]);
      break;
    default:
      break;
  }
  led_pattern++;

}