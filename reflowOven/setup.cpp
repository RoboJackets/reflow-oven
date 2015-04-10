#include "setup.h"

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

TouchScreen ts = TouchScreen(XP, YP, XM, YM,300);

int profile1[][2] = {{0,25}, {90,150}, {180,175}, {210,217}, {240,249}, {270,217}};
Profile profile = Profile(6,profile1,"SMD291SNL10");

bool isHeatOn = false;

uint16_t cFRONT = WHITE;
uint16_t cBACK = BLACK;

void heatOn() {
	digitalWrite(heat, HIGH);
	isHeatOn = true;
}

void heatOff() {
	digitalWrite(heat, LOW);
	isHeatOn = false;
}

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

Profile::Profile(int l, int (*val)[2], String n) {
	length = l;
	// values = (new int *)[2];
	values = val;
	name = n;
}