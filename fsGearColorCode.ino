#include <Arduino.h>
#include <FastLED.h>
//22
#define NUM_LEDS_GEAR 20
#define NUM_LEDS_RPM 24
#define NUM_LEDS_PER_GEAR 4
#define DATA_PIN_GEAR 5
#define DATA_PIN_RPM 2

CRGB gearColors[] = {CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::Yellow, CRGB::Purple};

CRGB ledsGear[NUM_LEDS_GEAR];
CRGB ledsRpm[NUM_LEDS_RPM];
CRGB* gear1_leds[NUM_LEDS_PER_GEAR];
CRGB* gear2_leds[NUM_LEDS_PER_GEAR];
CRGB* gear3_leds[NUM_LEDS_PER_GEAR];
CRGB* gear4_leds[NUM_LEDS_PER_GEAR];
CRGB* gear5_leds[NUM_LEDS_PER_GEAR];

int gear = 1;  // Default gear
int rpm = 0;   // Default RPM
int gearLightshowIndex = 1; //for lightshow
int rpmLightshowIndex = 1; //for lightshow
unsigned long previousRpmMillis = 0;
unsigned long previousGearMillis = 0;

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_GEAR>(ledsGear, NUM_LEDS_GEAR);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_RPM>(ledsRpm, NUM_LEDS_RPM);

  // Assign LED groups for each gear
  gear1_leds[0] = &ledsGear[0];
  gear1_leds[1] = &ledsGear[1];
  gear1_leds[2] = &ledsGear[19];
  gear1_leds[3] = &ledsGear[18];

  gear2_leds[0] = &ledsGear[2];
  gear2_leds[1] = &ledsGear[3];
  gear2_leds[2] = &ledsGear[17];
  gear2_leds[3] = &ledsGear[16];

  gear3_leds[0] = &ledsGear[4];
  gear3_leds[1] = &ledsGear[5];
  gear3_leds[2] = &ledsGear[15];
  gear3_leds[3] = &ledsGear[14];

  gear4_leds[0] = &ledsGear[6];
  gear4_leds[1] = &ledsGear[7];
  gear4_leds[2] = &ledsGear[13];
  gear4_leds[3] = &ledsGear[12];

  gear5_leds[0] = &ledsGear[8];
  gear5_leds[1] = &ledsGear[9];
  gear5_leds[2] = &ledsGear[11];
  gear5_leds[3] = &ledsGear[10];
  FastLED.setBrightness(255);

  Serial.println("Send a gear number (1-5) via Serial to control the LEDs.");
}

void loop() {
  lightshow();
  FastLED.clear();
  
  //serious stuff here
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');  // Read input until a newline
    input.trim();  // Remove any trailing spaces or newlines

    // Split the input into gear and rpm values
    int spaceIndex = input.indexOf(' ');
    if (spaceIndex == -1) {
      Serial.println("Invalid input! Please send in the format '<gear> <rpm>'");
      return;
    }

    String gearStr = input.substring(0, spaceIndex);
    String rpmStr = input.substring(spaceIndex + 1);

    gear = gearStr.toInt();
    rpm = rpmStr.toInt();

    // Validate gear and rpm values
    if (gear < 1 || gear > 5) {
      Serial.println("Invalid gear! Please send a gear number between 1 and 5.");
      return;
    }
    if (rpm < 0 || rpm > NUM_LEDS_RPM) {
      Serial.println("Invalid RPM! Please send an RPM number between 0 and 24.");
      return;
    }

    Serial.print("Gear: ");
    Serial.print(gear);
    Serial.print(", RPM: ");
    Serial.println(rpm);

    // Clear all LEDs
    FastLED.clear();

    // Light up the relevant LEDs based on the gear
    switch (gear) {
      case 5:
        for (int i = 0; i < NUM_LEDS_PER_GEAR; i++) {
          *gear5_leds[i] = CRGB::Pink;
        }
        break;
      case 4:
        for (int i = 0; i < NUM_LEDS_PER_GEAR; i++) {
          *gear4_leds[i] = CRGB::Pink;
        }
        break;
      case 3:
        for (int i = 0; i < NUM_LEDS_PER_GEAR; i++) {
          *gear3_leds[i] = CRGB::Pink;
        }
        break;
      case 2:
        for (int i = 0; i < NUM_LEDS_PER_GEAR; i++) {
          *gear2_leds[i] = CRGB::Pink;
        }
        break;
      case 1:
        for (int i = 0; i < NUM_LEDS_PER_GEAR; i++) {
          *gear1_leds[i] = CRGB::Pink;
        }
        break;
    }
    

    int led1 = 12 - rpm;       // First LED in the box
    int led2 = 11 + rpm;       // Second LED in the box
    ledsRpm[led1] = CRGB::Red;  // Light up the first LED
    ledsRpm[led2] = CRGB::Red;  // Light up the second LED

    for (int i = 1; i <= rpm; i++) {
      int led1 = 12 - i;         // First LED in the box
      int led2 = 11 + i;         // Second LED in the box
      if (i <= 2) {              // First range (Green)
        ledsRpm[led1] = CRGB::Green;
        ledsRpm[led2] = CRGB::Green;
      } else if (i <= 10) {       // Second range (Yellow)
        ledsRpm[led1] = CRGB::Yellow;
        ledsRpm[led2] = CRGB::Yellow;
      } else if (i <= 11) {                   // Third range (Red)
        ledsRpm[led1] = CRGB::Red;
        ledsRpm[led2] = CRGB::Red;
      } else{
        ledsRpm[led1] = CRGB::Blue;
        ledsRpm[led2] = CRGB::Blue;
      }
    }
    
    // Show the LED update
    FastLED.show();
  }
}

void lightshow() {
  for (int i = NUM_LEDS_RPM - 1; i >= 0; i--) {
    FastLED.clear();
    ledsRpm[i] = CRGB::HotPink;
    ledsRpm[i+1] = CRGB::HotPink;
    ledsRpm[i+2] = CRGB::HotPink;
    int ledIndexForGearBox = i%6;
    switch (ledIndexForGearBox) {
      case 5:
        for (int i = 0; i < NUM_LEDS_PER_GEAR; i++) {
          *gear5_leds[i] = CRGB::HotPink;
        }
        break;
      case 4:
        for (int i = 0; i < NUM_LEDS_PER_GEAR; i++) {
          *gear4_leds[i] = CRGB::HotPink;
        }
        break;
      case 3:
        for (int i = 0; i < NUM_LEDS_PER_GEAR; i++) {
          *gear3_leds[i] = CRGB::HotPink;
        }
        break;
      case 2:
        for (int i = 0; i < NUM_LEDS_PER_GEAR; i++) {
          *gear2_leds[i] = CRGB::HotPink;
        }
        break;
      case 1:
        for (int i = 0; i < NUM_LEDS_PER_GEAR; i++) {
          *gear1_leds[i] = CRGB::HotPink;
        }
        break;
    }
    FastLED.show();
    delay(70);
  }
}

