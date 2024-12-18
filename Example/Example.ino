#include <Adafruit_NeoPixel.h>


#define PIN 2  // input pin Neopixel is attached to

#define NUMPIXELS 300  // number of neopixels in strip

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int buffer[15];
int peak[15];



String incomingData = "";
uint32_t data[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0 };
uint32_t colorData[19] = { 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535 };
uint32_t bufferColor = 65535;
uint32_t peakColor = pixels.Color(255, 255, 255);

int StringCount = 0;

const long peakDecreaseTimer = 90;
unsigned long prePeakDecreaseTime = 0;

const long dataDecreaseTimer = 16;
unsigned long preDataDecreaseTime = 0;

const long animationTimer = 50;
unsigned long preAnimationTime = 0;

void setup() {

  for (int i = 0; i < 15; i++) {
    buffer[i] = i + 1;
    peak[i] = 20;
  }
  // Initialize the NeoPixel library.
  Serial.begin(115200);
  Serial.setTimeout(3);
  pixels.begin();
  pixels.show();
  pixels.setBrightness(15);
  showSpectrum();
}

void loop() {
  if (Serial.available() > 0) {
    incomingData = Serial.readStringUntil('\n');
    incomingData.trim();
    int index = 0;
    while ((index = incomingData.indexOf(' ')) != -1) {
      data[StringCount++] = incomingData.substring(0, index).toInt();
      incomingData = incomingData.substring(index + 1);
    }
    data[StringCount++] = incomingData.toInt();
    incomingData = "";
    StringCount = 0;

    switch (data[0]) {
      case 0:
        for (int i = 0; i < 5; i++) colorData[i] = data[i + 1];
        break;
      case 1:
        for (int i = 5; i < 10; i++) colorData[i] = data[i - 4];
        break;
      case 2:
        for (int i = 10; i < 15; i++) colorData[i] = data[i - 9];
        break;
      case 3:
        for (int i = 15; i < 19; i++) colorData[i] = data[i - 14];
        break;
      case 4:
         updateSpectrum();
        break;
    }
  }

  unsigned long currentTime = millis();
  if (currentTime - prePeakDecreaseTime >= peakDecreaseTimer) {
    decreasePeakBar();
    prePeakDecreaseTime = currentTime;
  }

}

void showSpectrum() {
  pixels.clear();
  for (int i = 0; i < 15; i++) {
    for (int j = 0; j < buffer[14 - i]; j++) {
      int index = (i % 2 == 0) ? i * 20 + j : (1 + i) * 20 - j - 1;
      bufferColor = colorData[j];
      pixels.setPixelColor(index, bufferColor);
    }
    int peakIndex = (i % 2 == 0) ? (i * 20) + peak[14 - i] - 1 : (1 + i) * 20 - peak[14 - i];
    pixels.setPixelColor(peakIndex, peakColor);
  }
  pixels.show();
}

void decreasePeakBar() {
  for (int i = 0; i < 15; i++) {
    if (peak[i] > 1 && peak[i] > buffer[i]) {
      peak[i] -= 1;
    }
  }
}

void updateSpectrum() {
  for (int i = 0; i < 15; i++) {
    if (buffer[i] < data[i + 1]) {
      buffer[i] = data[i + 1];
    } else if (buffer[i] > 0) {
      buffer[i] -= 1;
    }
    if (peak[i] < buffer[i]) {
      peak[i] = buffer[i] + 1;
    }
  }
  showSpectrum();
}
