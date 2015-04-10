#include <max6675.h>
#include <TouchScreen.h>
//#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "window.h"
// #include <EEPROM.h>

#define BAUDRATE 9600

int main(){

    Serial.begin(BAUDRATE);
    tft.begin();
    tft.setRotation(1); 
    tft.fillScreen(MAROON);
    isHeatOn = false;

    while(1){
        mainWindow();    
    }
}