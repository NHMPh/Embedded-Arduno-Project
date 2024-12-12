#include <Adafruit_NeoPixel.h>

#define PIN 2         // input pin Neopixel is attached to
#define NUMPIXELS 300 // number of neopixels in strip
#define NUM_COLUMNS 15

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const int delayval = 50;   // timing delay in milliseconds
const int fallDelay = 100; // delay for head falling down

// Define color constants as constant arrays
const int HEAD_COLOR[] = {255, 0, 0};   // Red
const int BASE_COLOR[] = {0, 255, 255}; // Cyan
const int OFF_COLOR[] = {0, 0, 0};      // Off/Black
String incomingData = "";
int StringCount = 0;
// Compact definition of LED start indices
const int ledStartIndex[] = {
    280, 279, 240, 239, 200, 199, 160, 159, 120, 119, 80, 79, 40, 39, 0};

// Input array to simulate external input
int inputArray[NUM_COLUMNS] = {0};

// Array to track the highest position for each column
int highestPositions[NUM_COLUMNS] = {0};

// Array to track head positions for each column
int headPositions[NUM_COLUMNS] = {0};

void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(3);
  pixels.begin();
  randomSeed(analogRead(0)); // Initialize random seed
  pixels.show();
}

void shuffleInputArray()
{
  // Fisher-Yates shuffle algorithm
  for (int i = NUM_COLUMNS - 1; i > 0; i--)
  {
    int j = random(i + 1);
    int temp = inputArray[i];
    inputArray[i] = inputArray[j];
    inputArray[j] = temp;
  }
}

void updateColumn(int columnIndex)
{
  // Validate input
  if (columnIndex < 0 || columnIndex >= NUM_COLUMNS)
    return;

  // Get the start index for this column
  int startIndex = ledStartIndex[columnIndex];

  // Determine direction based on column index
  bool goingUp = (columnIndex % 2 == 0);

  // Get the intensity for this column from inputArray
  int intensity = inputArray[columnIndex];

  // Clear the entire column first
  for (int j = 0; j < 20; j++)
  {
    int ledPosition = goingUp ? (startIndex + j) : (startIndex - j);
    pixels.setPixelColor(ledPosition, pixels.Color(OFF_COLOR[0], OFF_COLOR[1], OFF_COLOR[2]));
  }

  // Light up the column based on intensity
  if (intensity > 0)
  {
    // Update highest position for this column
    highestPositions[columnIndex] = intensity;
    headPositions[columnIndex] = intensity - 1;

    // Light up the head LED immediately
    int headPosition = goingUp ? (startIndex + intensity - 1) : (startIndex - intensity + 1);
    pixels.setPixelColor(headPosition, pixels.Color(HEAD_COLOR[0], HEAD_COLOR[1], HEAD_COLOR[2]));

    // Light up the body of the column (excluding the head)
    for (int j = 0; j < intensity - 1; j++)
    {
      int ledPosition = goingUp ? (startIndex + j) : (startIndex - j);
      pixels.setPixelColor(ledPosition, pixels.Color(BASE_COLOR[0], BASE_COLOR[1], BASE_COLOR[2]));
    }
  }

  pixels.show();
}

void fallColumns()
{
  bool columnsActive = true;
  while (columnsActive)
  {
    columnsActive = false;
    for (int i = 0; i < NUM_COLUMNS; i++)
    {
      int startIndex = ledStartIndex[i];
      bool goingUp = (i % 2 == 0);
      int highestPosition = highestPositions[i];

      if (highestPosition > 0)
      {
        columnsActive = true;

        // Clear the entire column first
        for (int k = 0; k < 20; k++)
        {
          int ledPosition = goingUp ? (startIndex + k) : (startIndex - k);
          pixels.setPixelColor(ledPosition, pixels.Color(OFF_COLOR[0], OFF_COLOR[1], OFF_COLOR[2]));
        }

        // Find the new head position
        int newHeadPosition = goingUp ? (startIndex + headPositions[i]) : (startIndex - headPositions[i]);

        // Keep the head LED at the bottom
        if (headPositions[i] == 0)
        {
          pixels.setPixelColor(newHeadPosition, pixels.Color(HEAD_COLOR[0], HEAD_COLOR[1], HEAD_COLOR[2]));
        }

        // Relight the body up to current height (if applicable)
        if (headPositions[i] > 0)
        {
          for (int k = 0; k < headPositions[i]; k++)
          {
            int ledPosition = goingUp ? (startIndex + k) : (startIndex - k);
            pixels.setPixelColor(ledPosition, pixels.Color(BASE_COLOR[0], BASE_COLOR[1], BASE_COLOR[2]));
          }

          // Set the new head LED before it reaches bottom
          int newLedHeadPosition = goingUp ? (startIndex + headPositions[i] - 1) : (startIndex - headPositions[i] + 1);
          pixels.setPixelColor(newLedHeadPosition, pixels.Color(HEAD_COLOR[0], HEAD_COLOR[1], HEAD_COLOR[2]));
        }

        headPositions[i]--;
        highestPositions[i]--;
      }
    }
    pixels.show();
    delay(fallDelay);
  }

  // Fully turn off all columns
  for (int i = 0; i < NUM_COLUMNS; i++)
  {
    int startIndex = ledStartIndex[i];
    bool goingUp = (i % 2 == 0);
    for (int j = 0; j < 20; j++)
    {
      int ledPosition = goingUp ? (startIndex + j) : (startIndex - j);
      pixels.setPixelColor(ledPosition, pixels.Color(OFF_COLOR[0], OFF_COLOR[1], OFF_COLOR[2]));
    }
  }
  pixels.show();

  // Reset highest positions and head positions
  for (int i = 0; i < NUM_COLUMNS; i++)
  {
    highestPositions[i] = 0;
    headPositions[i] = 0;
  }
}

void loop()
{
  // Randomize the input array to simulate different frequencies
  if (Serial.available() > 0)
  {
    incomingData += Serial.readString();
  }

  while (incomingData.length() > 0)
  {
    incomingData.replace("\n", "");
    incomingData.trim();
    int index = incomingData.indexOf(' ');

    if (index == -1) // No space found
    {
      inputArray[StringCount++] = incomingData.toInt();
      break;
    }
    else
    {
      inputArray[StringCount++] = incomingData.substring(0, index).toInt();
      incomingData = incomingData.substring(index + 1);
    }
  }

  if (incomingData.length() > 0)
  {

    StringCount = 0;
    incomingData = "";
    // Update and display each column
    for (int i = 0; i < NUM_COLUMNS; i++)
    {
      updateColumn(i);
    }
  }

  fallColumns();

  // Fall down effect for all columns simultaneously
}