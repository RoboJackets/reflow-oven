#include <max6675.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include <TouchScreen.h>


#include "windows.h"

int main() {
	Serial.begin(9600);
    tft.begin();
    tft.setRotation(1); 
    tft.fillScreen(MAROON);
    isHeatOn = 0;
    heatOn(0);

    while (true) {
    	mainWindow();
    }
}