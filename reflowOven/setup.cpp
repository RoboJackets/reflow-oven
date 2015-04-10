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

// EEPROM/profile functions not functioning yet
// void EEPROM_put(int start, int length, int size, int *elems) {
// 	for (int i=0; i<length; i++) {
// 		for (int j=0; j<size; j++) {
// 			EEPROM.write(start+i*size+j,elems[i]/pow(256,size)+elems[i]%int(pow(256,size)));
// 		}
// 	}
// }

// int* EEPROM_get(int start, int length, int size) {
// 	size_t count = 0;
// 	int *elem1 = new int[50];
// 	for (int i=0; i<length; i++) {
// 		int out = 0;
// 		for (int j=0; j<size; j++) {
// 			out += EEPROM.read(start+i*size+j)*pow(256,j);
// 		}
// 		elem1[count++] = out;
// 	}

// 	int *elem2 = new int[count];
// 	for (int i=0; i<count; i++) {
// 		elem2[i] = elem1[i];
// 		// Serial.println(elem2[i]);
// 	}

// 	return elem2;
// }



// // void test5() {
// // 	tft.fillScreen(WHITE);

// // 	clearProfiles();

// // 	String text = "abc";
// // 	int newText[text.length()];
// // 	for (int i=0; i<text.length(); i++) {
// // 		newText[i] = text[i];
// // 	}

// // 	tft.setCursor(0,0);
// // 	EEPROM_put(0,3,1,newText);

// // 	int *intOut = EEPROM_get(0,3,1);
// // 	String textOut = "";
// // 	for (int i=0; i<text.length(); i++) {
// // 		Serial.println(char(intOut[i]));
// // 		textOut += char(intOut[i]);
// // 	}

// // 	tft.println(textOut);
// // }



// void addProfile(Profile profile) {
// 	clearProfiles();

// 	int pos = 2;

// 	int name[profile.name.length()];
// 	for (int i=0; i<profile.name.length(); i++) {
// 		name[i] = profile.name[i];
// 	}

// 	int *nums = (int*)&profile.values;

// 	// int length1[] = {profile.name.length()};
// 	// EEPROM_put(pos, 1, 2, length1);
// 	EEPROM.write(pos,profile.name.length());
// 	pos += 1;
// 	EEPROM_put(pos, profile.name.length(), 1, name);
// 	pos += profile.name.length();

// 	// int length2[] = {profile.length};
// 	// EEPROM_put(pos, 1, 2, length2);
// 	EEPROM.write(pos,profile.length);
// 	pos += 2;
// 	EEPROM_put(pos, profile.length, 2, nums);
// 	pos += profile.length;

// }

// Profile readProfile(int num) {
// 	int current = 0;
// 	int pos = 2;

// 	int nameLength;
// 	int profileLength;

// 	// while (current < num) {
// 	// 	nameLength = EEPROM_get(pos,1,2)[0];
// 	// 	pos += 2 + nameLength;

// 	// 	profileLength = EEPROM_get(pos,1,2)[0];
// 	// 	pos += 2 + profileLength;

// 	// 	current++;
// 	// }

// 	nameLength = EEPROM.read(pos);
// 	pos += 1;

// 	int *textOut = EEPROM_get(pos,nameLength,1);
// 	String name = "";
// 	for (int i=0; i<nameLength; i++) {
// 		name += char(textOut[i]);
// 	}
// 	pos += nameLength;

// 	profileLength = EEPROM.read(pos);

// 	// tft.setTextSize(1);

// 	// for (int i=0; i<512; i++) {
// 	// 	tft.setCursor(0,0);
// 	// 	tft.fillRect(0,0,5*9,8,WHITE);
// 	// 	tft.print(String(i)+","+String(EEPROM.read(i)));
// 	// 	delay(100);
// 	// }

// 	// tft.println(EEPROM.read(2));
// 	// tft.println(EEPROM.read(3));

// 	// tft.println(nameLength);
// 	// tft.println(name);
// 	// tft.println(profileLength);

// 	// tft.print(3);

// 	pos += 2;
// 	int *test = EEPROM_get(pos,profileLength,2);

// 	// tft.print(4);

// 	int (*intOut)[2] = (int (*)[2])&test[0];
// 	// intOut = EEPROM_get(pos,profileLength,2);


// 	return Profile(profileLength,intOut,name);
// }

// // String readProfileName(int num) {
// // 	int current = 0;
// // 	int pos = 2;

// // 	int nameLength;

// // 	nameLength = EEPROM_get(pos,1,2)[0];
// // 	pos += 2;
// // 	int *intOut = EEPROM_get(pos,nameLength,1);
// // 	String text = "";
// // 	for (int i=0; i<nameLength; i++) {
// // 		text += char(intOut[i]);
// // 	}

// // 	return text;
// // }

// void deleteProfile(int num) {
// }

// void clearProfiles() {
// 	for (int i = 0; i<256; i++) {
// 		EEPROM.write(i,255);
// 	}
// }