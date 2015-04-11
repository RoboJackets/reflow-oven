#include "Arduino.h"

#include "max6675.h"

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#include "TouchScreen.h"


#define MAX_START_TEMP 100


//A0  - Temperature control

//MAX6675
///////////
//D04 - DO
//D05 - CS
//D06 - SCK

//Touchscreen
////////////
//D02 - SDCS
//D07 - RESET
//D08 - D/C
//D09 - LITE
//D10 - LCDCS
//D11 - MOSI
//D12 - MISO
//D13 - SCLK
//A1  - Y+
//A2  - X+
//A3  - Y-
//A4  - X-
//5V  - Vin

///////////////
//D03 - Speaker
//TX1 - Serial
//RX0 - Serial
//A5  - LED:1
//A6  - LED:2/CDT ?
//A7  - LED:3
//RST - RST:External

#define heatUp    A0
#define heatDown  A5
#define speaker   3

extern MAX6675 thermocouple;
#define thermoDO   4
#define thermoCS   5
#define thermoCLK  6

extern Adafruit_ILI9341 tft;
#define TFT_CS   10
#define TFT_DC   8

//240x320
#define TFT_XMAX 320
#define TFT_YMAX 240

extern TouchScreen ts;
#define YP A1
#define XM A4
#define YM A3
#define XP A2

// Touch screen calibration
#define TS_MINX 100
#define TS_MINY 85
#define TS_MAXX 900
#define TS_MAXY 900

#define BLACK       0x0000      /*   0,   0,   0 */
#define NAVY        0x000F      /*   0,   0, 128 */
#define DARKGREEN   0x03E0      /*   0, 128,   0 */
#define DARKCYAN    0x03EF      /*   0, 128, 128 */
#define MAROON      0x7800      /* 128,   0,   0 */
#define PURPLE      0x780F      /* 128,   0, 128 */
#define OLIVE       0x7BE0      /* 128, 128,   0 */
#define LIGHTGREY   0xC618      /* 192, 192, 192 */
#define DARKGREY    0x7BEF      /* 128, 128, 128 */
#define BLUE        0x001F      /*   0,   0, 255 */
#define GREEN       0x07E0      /*   0, 255,   0 */
#define CYAN        0x07FF      /*   0, 255, 255 */
#define RED         0xF800      /* 255,   0,   0 */
#define MAGENTA     0xF81F      /* 255,   0, 255 */
#define YELLOW      0xFFE0      /* 255, 255,   0 */
#define WHITE       0xFFFF      /* 255, 255, 255 */
#define ORANGE      0xFD20      /* 255, 165,   0 */
#define GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define PINK        0xF81F      /* 248,  31, 255 */

#define tSize  2

void heatOn(int val);
double getTemp();
// void heatOff();

int freeRam();

struct ReflowPhase {
  char* Name;
  int ExitTemperatureC;
  int MinDurationS;
  int MaxDurationS;
  int TargetDurationS;
  bool AlarmOnExit;
};

struct ReflowProfile {
  char* Name;
  ReflowPhase Phases[4];
};

class Profile {
	public:
		int length;
		int (*values)[2];
		String name;
		Profile(int, int (*)[2], String);
};

extern int isHeatOn;
extern Profile profile;
extern uint16_t cFRONT;
extern uint16_t cBACK;
