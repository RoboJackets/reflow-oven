#include "setup.h"

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

ReflowProfile profile = profile11;


//ReflowProfile profile1 =
//{
//	"SMD291SNL10", 80, 5,
//	{
//		//   Zone      Exit(C)   Min(S)  Max(S)  Tgt(S)  Alarm
//		{ "Pre-heat",     140,       0,      0,     90,  false },
//		{ "Soak",         217,      60,     90,     80,  false },
//		{ "Liquidus",     240,      15,     30,     20,  false },
//		{ "Reflow",       217,      15,     30,     20,  true  },
//		{ "Cooling",      80,        0,      0,     80,  true  },
//    }
//};

ReflowProfile profile11 =
 {
     "SMD291SNL10", 80, 6,
     {
         //   Zone      Exit(C)   Min(S)  Max(S)  Tgt(S)  Alarm
         { "Pre-heat",     130,       0,      0,     70,  false },
         { "Pre-heat-2",   140,      70,    100,     80,  false },
         { "Soak",         217,      60,     90,     80,  false },
         { "Liquidus",     240,      25,     40,     30,  false },
         { "Reflow",       217,      25,     35,     30,  true  },
         { "Cooling",      80,        0,      0,     80,  true  },
     }
 };


//ReflowProfile profile2 =
//{
//	"TEST", 80, 5,
//	{
//		//   Zone      Exit(C)   Min(S)  Max(S)  Tgt(S)  Alarm
//		{ "Pre-heat",     150,       0,      0,     90,  false },
//		{ "Soak",         217,      60,     90,    100,  false },
//		{ "Liquidus",     250,      15,     30,     20,  false },
//		{ "Reflow",       217,      15,     30,     20,  true  },
//		{ "Cooling",      25,        0,      0,     90,  true  },
//    }
//};

int isHeatOn = 0;
long lastHeat = millis();
double tHeat = 0;

uint16_t cFRONT = WHITE;
uint16_t cBACK = BLACK;

void heatOn(int val) {
	isHeatOn = val;

	// 0 - heat off
	// 1 - bottom only
	// 2 - top only
	// 3 both on
	if (val==1 || val==3) {
		digitalWrite(heatDown, HIGH);
	} else {
		digitalWrite(heatDown,LOW);
	}
	if (val==2 || val==3) {
		digitalWrite(heatUp, HIGH);
	} else {
		digitalWrite(heatUp,LOW);
	}
}

double getTemp() {
	// Calibration for thin thermocouple
	return thermocouple.readCelsius()*1.0944 - 20.506;
}


// int freeRam () {
//   extern int __heap_start, *__brkval;
//   int v;
//   return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
// }
