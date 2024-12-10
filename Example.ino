#include <Adafruit_NeoPixel.h>

#define PIN 2         // input pin Neopixel is attached to
#define NUMPIXELS 300 // number of neopixels in strip

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const int delayval = 20; // timing delay in milliseconds

int redColor = 0;
int greenColor = 0;
int blueColor = 0;

const int headRedColor = 255; // Head LED color
const int headGreenColor = 0;
const int headBlueColor = 0;

const int defaultRedColor = 0; // Cyan base color
const int defaultGreenColor = 255;
const int defaultBlueColor = 255;

const int ledStartIndex[] = {
    280, 279, 240, 239, 200, 199, 160, 159, 120, 119, 80, 79, 40, 39, 0};

int previousHeadLed = -1; // Variable to store the previous head LED

void setup()
{
  pixels.begin();
  randomSeed(analogRead(0)); // Initialize random seed

  pixels.show();
}

void ledRun(int ledColumnIndex)
{
  // Validate input
  if (ledColumnIndex < 0 || ledColumnIndex >= 15)
    return;

  // Get the start index for this column
  int startIndex = ledStartIndex[ledColumnIndex];

  // Determine direction based on column index
  bool goingUp = (ledColumnIndex % 2 == 0);

  // Run the light effect
  int currentLed;
  for (int j = 0; j < 20; j++)
  {
    // Calculate current LED position based on direction
    currentLed = goingUp ? (startIndex + j) : (startIndex - j);

    // If this is not the first LED in the run
    if (j > 0)
    {
      // Calculate the previous LED
      int prevLed = goingUp ? (startIndex + j - 1) : (startIndex - j + 1);

      // Set previous LED back to default color
      pixels.setPixelColor(prevLed, pixels.Color(defaultRedColor, defaultGreenColor, defaultBlueColor));
    }

    // Set current LED to head color
    pixels.setPixelColor(currentLed, pixels.Color(headRedColor, headGreenColor, headBlueColor));

    pixels.show();
    delay(delayval);
  }
}

void setColor()
{
  redColor = random(0, 255);
  greenColor = random(0, 255);
  blueColor = random(0, 255);
}

void loop()
{
  setColor();

  // Run through all columns
  for (int i = 0; i < 15; i++)
  {
    ledRun(i);
  }
}