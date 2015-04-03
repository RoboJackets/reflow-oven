#include "setup.h"

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

TouchScreen ts = TouchScreen(XP, YP, XM, YM,300);

bool isHeatOn = false;

void heatOn() {
	digitalWrite(heat, HIGH);
	isHeatOn = true;
}

void heatOff() {
	digitalWrite(heat, LOW);
	isHeatOn = false;
}

void EEPROM_put(int start, int length,int size, int *elems) {
	for (int i=0; i<length; i++) {
		for (int j=0; j<size; j++) {
			EEPROM.write(start+i*size+j,elems[i]/pow(256,size)+elems[i]%int(pow(256,size)));
		}
	}
}

int* EEPROM_get(int start, int length, int size) {
	size_t count = 0;
	int *elem1 = new int[50];
	for (int i=0; i<length; i++) {
		int out = 0;
		for (int j=0; j<size; j++) {
			out += EEPROM.read(start+i*size+j)*pow(256,j);
		}
		elem1[count++] = out;
	}

	int *elem2 = new int[count];
	for (int i=0; i<count; i++) {
		elem2[i] = elem1[i];
		Serial.println(elem2[i]);
	}

	return elem2;
}
