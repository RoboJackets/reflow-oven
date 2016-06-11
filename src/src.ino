#include <SPI.h>
#include "windows.h"

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Wire.h>
#include <Adafruit_FT6206.h>

void setup() {
  Serial.begin(57600);
  
  tft.begin();

  if (! ts.begin(40)) {  // pass in 'sensitivity' coefficient
    Serial.println("Couldn't start FT6206 touchscreen controller");
    while (1);
  }

  tft.setRotation(3);
  tft.fillScreen(MAROON);

  pinMode(heatUp, OUTPUT);
  pinMode(heatDown, OUTPUT);
  pinMode(13, OUTPUT);

  heatOn(0);
}

void loop() {
  mainWindow();
}
