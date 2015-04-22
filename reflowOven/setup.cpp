#include "setup.h"

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);



ReflowProfile profile = 
{
	"SMD291SNL10", 80, 5,
	{
		//   Zone      Exit(C)   Min(S)  Max(S)  Tgt(S)  Alarm
		{ "Pre-heat",     150,       0,      0,     90,  false },
		{ "Soak",         217,      60,     90,     80,  false },
		{ "Liquidus",     240,      15,     30,     20,  false },
		{ "Reflow",       217,      15,     30,     20,  true  },
		{ "Cooling",      80,        0,      0,     80,  true  },
    }
};


int isHeatOn = 0;
long lastHeat = millis();
double tHeat = 0;

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
	// Calibration for thin thermocouple
	return thermocouple.readCelsius()*1.0944 - 20.506; 
}


int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}