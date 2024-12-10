#include <Adafruit_NeoPixel.h>

#define PIN 2 // input pin Neopixel is attached to
#define NUMPIXELS 300 // number of neopixels in strip

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const int delayval = 50; // timing delay in milliseconds

int redColor = 0;
int greenColor = 0;
int blueColor = 0;

const int headRedColor = 255; // Head LED color
const int headGreenColor = 0;
const int headBlueColor = 0;

const int ledStartIndex[] = {
    280, 279, 240, 239, 200, 199, 160, 159, 120, 119, 80, 79, 40, 39, 0
};

void setup() {
  pixels.begin();
  randomSeed(analogRead(0)); // Initialize random seed
}

void ledRun(int ledColumnIndex) {
  // Validate input
  if (ledColumnIndex < 0 || ledColumnIndex >= 15) return;
  
  // Get the start index for this column
  int startIndex = ledStartIndex[ledColumnIndex];
  
  // Determine direction based on column index
  bool goingUp = (ledColumnIndex % 2 == 0);
  
  // Clear previous column's lights first
  clearColumn(ledColumnIndex);
  
  // Run the light effect
  if (goingUp) {
    // Run upwards
    for (int j = 0; j < 20; j++) {
      int currentLed = startIndex + j;
      
      // Set head LED (first LED) with different color
      if (j == 0) {
        pixels.setPixelColor(currentLed, pixels.Color(headRedColor, headGreenColor, headBlueColor));
      } else {
        pixels.setPixelColor(currentLed, pixels.Color(redColor, greenColor, blueColor));
      }
      
      pixels.show();
      delay(delayval);
    }
  } else {
    // Run downwards
    for (int j = 0; j < 20; j++) {
      int currentLed = startIndex - j;
      
      // Set head LED (first LED) with different color
      if (j == 0) {
        pixels.setPixelColor(currentLed, pixels.Color(headRedColor, headGreenColor, headBlueColor));
      } else {
        pixels.setPixelColor(currentLed, pixels.Color(redColor, greenColor, blueColor));
      }
      
      pixels.show();
      delay(delayval);
    }
  }
}

void clearColumn(int ledColumnIndex) {
  // Validate input
  if (ledColumnIndex < 0 || ledColumnIndex >= 15) return;
  
  int startIndex = ledStartIndex[ledColumnIndex];
  bool goingUp = (ledColumnIndex % 2 == 0);
  
  // Clear 20 LEDs in the column
  for (int j = 0; j < 20; j++) {
    int currentLed = goingUp ? (startIndex + j) : (startIndex - j);
    pixels.setPixelColor(currentLed, pixels.Color(0, 0, 0)); // Turn off
  }
  pixels.show();
}

void setColor() {
  redColor = random(0, 255);
  greenColor = random(0, 255);
  blueColor = random(0, 255);
}

void loop() {
  setColor();
  
  // Run through all columns
  for (int i = 0; i < 15; i++) {
    ledRun(i);
    delay(250);
  }
}