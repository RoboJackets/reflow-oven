#include "setup.h"

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

TouchScreen ts = TouchScreen(XP, YP, XM, YM,300);


int profile1[][2] = {{0,25}, {90,150}, {180,175}, {210,217}, {240,249}, {270,217}};
Profile profile = Profile(6,profile1,"SMD291SNL10");


ReflowProfile profileTest = 
{
	"SMD291SNL10",
	{
		//   Zone      Exit(C)   Min(S)  Max(S)  Tgt(S)  Alarm
		{ "Pre-heat",     150,       0,      0,     90,  false },
		{ "Soak",         217,      60,     90,     80,  false },
		{ "Liquidus",     240,      15,     30,     20,  false },
		{ "Reflow",       217,      15,     30,     20,  true }
    }
};

ReflowPhase idlePhase    = { "Idle",                 0, 0, 0, 0, false };
ReflowPhase coolingPhase = { "Cooling", MAX_START_TEMP, 0, 0, 0, false };


int isHeatOn = 0;

uint16_t cFRONT = WHITE;
uint16_t cBACK = BLACK;

void heatOn(int val) {
	isHeatOn = val;
	if (val==0) {
		digitalWrite(heatUp, LOW);
		digitalWrite(heatDown, LOW);
	} else if (val==1) {
		digitalWrite(heatUp,LOW);
		digitalWrite(heatDown,HIGH);
	} else if (val==2) {
		digitalWrite(heatUp, HIGH);
		digitalWrite(heatDown,LOW);
	} else if (val==3) {
		digitalWrite(heatUp, HIGH);
		digitalWrite(heatDown, HIGH);
	}
}

double getTemp() {
	return thermocouple.readCelsius();
}

// void heatOff() {
// 	digitalWrite(heatUp, LOW);
// 	isHeatOn = false;
// }

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