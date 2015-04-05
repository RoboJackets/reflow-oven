#include <max6675.h>

#include <TouchScreen.h>
//
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// #include <EEPROM.h>


#include "windows.h"

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
