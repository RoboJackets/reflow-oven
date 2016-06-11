#include <SPI.h>
#include "windows.h"

void setup() {
  Serial.begin(57600);
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(MAROON);
//  ts.begin(40);

  pinMode(heatUp, OUTPUT);
  pinMode(heatDown, OUTPUT);
  pinMode(13, OUTPUT);
  heatOn(0);
}

void loop() {
  Serial.println("hey");
  digitalWrite(13, false);
  mainWindow();
}
