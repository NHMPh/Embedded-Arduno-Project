#include <Adafruit_NeoPixel.h>

#define PIN 2         // input pin Neopixel is attached to
#define NUMPIXELS 300 // number of neopixels in strip

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const int delayval = 20; // timing delay in milliseconds

// Define color constants as constant arrays
const int HEAD_COLOR[] = {255, 0, 0};   // Red
const int BASE_COLOR[] = {0, 255, 255}; // Cyan

// Compact definition of LED start indices
const int ledStartIndex[] = {
    280, 279, 240, 239, 200, 199, 160, 159, 120, 119, 80, 79, 40, 39, 0};

int redColor = 0;
int greenColor = 0;
int blueColor = 0;

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
      pixels.setPixelColor(prevLed, pixels.Color(BASE_COLOR[0], BASE_COLOR[1], BASE_COLOR[2]));
    }

    // Set current LED to head color
    pixels.setPixelColor(currentLed, pixels.Color(HEAD_COLOR[0], HEAD_COLOR[1], HEAD_COLOR[2]));

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