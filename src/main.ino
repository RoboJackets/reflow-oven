#include <SPI.h>
#include "windows.h"

void setup() {
  Serial.begin(9600);
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(MAROON);
  ctp.begin(40);

  pinMode(heatUp, OUTPUT);
  pinMode(heatDown, OUTPUT);
  heatOn(0);
}

void loop() {
  mainWindow();
}
