#include <max6675.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include <TouchScreen.h>


#include "windows.h"

void setup() {
    Serial.begin(9600);
    tft.begin();
    tft.setRotation(3); 
    tft.fillScreen(MAROON);

    pinMode(heatUp, OUTPUT);
    pinMode(heatDown, OUTPUT);
    heatOn(0);
}

void loop() {
	mainWindow();
}