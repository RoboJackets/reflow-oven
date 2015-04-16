#include "setup.h"

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);


// int profile1[][2] = {{0,25}, {90,150}, {180,175}, {210,217}, {240,249}, {270,217}};
// Profile profile = Profile(6,profile1,"SMD291SNL10");


ReflowProfile profile = 
{
	"SMD291SNL10", 80, 4,
	{
		//   Zone      Exit(C)   Min(S)  Max(S)  Tgt(S)  Alarm
		{ "Pre-heat",     150,       0,      0,     90,  false },
		{ "Soak",         217,      60,     90,     80,  false },
		{ "Liquidus",     240,      15,     30,     20,  false },
		{ "Reflow",       217,      15,     30,     20,  true }
    }
};

// ReflowPhase idlePhase    = { "Idle",                 0, 0, 0, 0, false };
// ReflowPhase coolingPhase = { "Cooling", MAX_START_TEMP, 0, 0, 0, false };


int isHeatOn = 0;
long lastHeat = millis();
double tHeat = 0;

uint16_t cFRONT = WHITE;
uint16_t cBACK = BLACK;

void heatOn(int val) {
	if (isHeatOn==0) {
		tHeat -= double(millis()-lastHeat)/1000;
	} else {
		tHeat += double(millis()-lastHeat)/1000;
	}
	if (tHeat<0) {
		tHeat = 0;
	} else if (tHeat>HEAT_UP_TIME) {
		tHeat = HEAT_UP_TIME;
	}
	lastHeat = millis();

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

	// Works for temps from 0 C to 320 C
	// (0,0)	(100,118)	(200,227)	(320,320)
	// [-3.068e-6	+5e-4		+1.1636]
	// (0,0)	(118,100)	(227,185)	(320,320)
	// [+4.8e-6		-1.3e-3		+0.9398]

	double t = thermocouple.readCelsius();

	double heatU = -3.069e-6*pow(t,3) + 5e-4*pow(t,2) + 1.1536*t;
	// heat += map(heat-t,0,tHeat,0,heat-t);
	double heatD = 4.8e-6*pow(t,3) + -1.3e-3*pow(t,2) + 0.9398*t;
	// heat += map(heat-t,0,tHeat,0,heat-t);

	// double diff = map(heatU-heatD,0,tHeat,0,heat-t);
	double diff = map(tHeat,0,HEAT_UP_TIME,0,heatU-heatD);

	return heatU - diff;

	// double diff = map(heat-t,0,tHeat,0,heat-t);
	// if ((millis()-lastHeat)/1000 < 20) {
	// 	diff = map(heat-t,0,(millis()-lastHeat)/1000.0,0,heat-t);
	// 	return t+diff;
	// }

	// return t + diff;
}


int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

// Profile::Profile(int l, int (*val)[2], String n) {
// 	length = l;
// 	values = val;
// 	name = n;
// }