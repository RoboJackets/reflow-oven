#include <max6675.h>
#include <TouchScreen.h>
//#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "windows.h"
// #include <EEPROM.h>

void setup() {
    Serial.begin(9600);
    tft.begin();
    tft.setRotation(1); 
    tft.fillScreen(MAROON);
    isHeatOn = false;
}

void loop() {
	mainWindow();
}