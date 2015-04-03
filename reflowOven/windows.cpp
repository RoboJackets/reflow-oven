#include "windows.h"

// Start button
// View/add Profiles Button
// Current profile info
// Restart button

#define border 10
#define tSize  2

void graphProfile(int x, int y, int w, int h, double profile[][2], bool detailed) {
	int len = 6; // Find a way to find profile length

	double maxTemp = profile[0][1];
	double maxTime = profile[len-1][0];
	for (int i = 1; i<len-1; i++) {
		if (profile[i][1]>maxTemp) {
			maxTemp = profile[i][1];
		}
	}

	for (int i = 0; i<len-1; i++) {
		int x1 = map(long(profile[i][0]),0,long(maxTime),0,long(w));
		int y1 = map(long(profile[i][1]),0,long(maxTemp),0,long(h));

		int x2 = map(long(profile[i+1][0]),0,long(maxTime),0,long(w));
		int y2 = map(long(profile[i+1][1]),0,long(maxTemp),0,long(h));

		if (detailed) {
			tft.drawLine(x+x1,y+h-3,x+x1,y+h-y1,LIGHTGREY); // vertical lines
			tft.drawLine(x-3,y+h-y1,x+x1,y+h-y1,LIGHTGREY); // horizontal lines

			if (i==len-2) {
				tft.drawLine(x-3,y+h-y2,x+x2,y+h-y2,LIGHTGREY);
			}
		}

		tft.drawLine(x+x1,y+h-y1,x+x2-1,y+h-y2,BLACK);

		tft.drawLine(x+x1,y+h-3,x+x1,y+h+3,BLACK);
		tft.drawLine(x-3,y+h-y1,x+3,y+h-y1,BLACK);

		if (i==0 || detailed) {
			

			String time1 = String(int(profile[i][0]));
			tft.setCursor(x+x1-time1.length()*2.5,y+h+5);
			tft.setTextSize(1);
			tft.println(time1);

			String temp1 = String(int(profile[i][1]));
			tft.setCursor(x-3-temp1.length()*5-3,y+h-y1-8/2+2);
			tft.println(temp1);
		}
		if (i==len-2) {
			tft.drawLine(x+x2-1,y+h-3,x+x2-1,y+h+3,BLACK);
			tft.drawLine(x-3,y+h-y2,x+3,y+h-y2,BLACK);

			String time1 = String(int(profile[i+1][0]));
			tft.setCursor(x+x2-time1.length()*2.5,y+h+5);
			tft.setTextSize(1);
			tft.println(time1);
		}

		if (profile[i][1] == maxTemp) {
			String temp1 = String(int(profile[i][1]));
			tft.setCursor(x-3-temp1.length()*5-3,y+h-y1-8/2+2);
			tft.println(temp1);
		}

	}
}

void mainWindow() {
	tft.fillScreen(WHITE);

	int bWidth = (tft.width()-border*3)*.5;  //(320-45)*.5 = 137.5
	int startH = (tft.height()-border*3)*.5; //(240-45)*.5 = 97.5
	int otherH = (startH-border)*.5;         // (97-15)*.5 = 41

	int bStart [4] = {border,         border,                bWidth,startH};
	int bProfi [4] = {border,         startH+border*2,       bWidth,otherH};
	int bCusto [4] = {border,         startH+otherH+border*3,bWidth,otherH};
	int bResta [4] = {border*2+bWidth,startH+otherH+border*3,bWidth,otherH};

	tft.setTextColor(BLACK);
	tft.setTextSize(tSize); // Size 3: 5x8 * 3 = 15x24

	// Start Button
	tft.drawRoundRect(bStart[0],bStart[1],bStart[2],bStart[3],7,BLACK);
	tft.setCursor(bStart[0]+(bStart[2]-tSize*6*5)*.5,bStart[1]+(bStart[3]-tSize*8)*.5);
	tft.println("START");
	// Profiles Button
	tft.drawRoundRect(bProfi[0],bProfi[1],bProfi[2],bProfi[3],7,BLACK);
	tft.setCursor(bProfi[0]+(bProfi[2]-tSize*8*5)*.5,bProfi[1]+(bProfi[3]-tSize*8)*.5);
	tft.println("Profile");
	// Custom Controls Button
	tft.drawRoundRect(bCusto[0],bCusto[1],bCusto[2],bCusto[3],7,BLACK);
	tft.setCursor(bCusto[0]+(bCusto[2]-tSize*7*5)*.5,bCusto[1]+(bCusto[3]-tSize*8)*.5);
	tft.println("Custom");
	// Restart Button
	tft.drawRoundRect(bResta[0],bResta[1],bResta[2],bResta[3],7,BLACK);
	tft.setCursor(bResta[0]+(bResta[2]-tSize*8*5)*.5,bResta[1]+(bResta[3]-tSize*8)*.5);
	tft.println("Restart");


	int graph [4] = {border*3.5+bWidth,border*2+tSize*8,bWidth-border*1.8,startH-tSize*8-border};

	tft.drawRect(graph[0],graph[1],graph[2],graph[3],BLACK);
	// tft.setCursor(graph[0],border+tSize*8);
	tft.setCursor(graph[0]-7,border);
	tft.println("Profile Info");

	tft.setCursor(graph[0]+3,graph[1]-8);
	tft.setTextSize(1);
	tft.println(PROFILE1N);

	double profile[][2] = PROFILE1;
	graphProfile(graph[0],graph[1],graph[2],graph[3],profile,false);

	tft.setCursor(tft.width()/2+border,bProfi[1]+bProfi[3]/2);
	tft.setTextSize(2);
	tft.println("Temp:");
	int tempOld = thermocouple.readCelsius();
	tft.setCursor(tft.width()/2+border+5*6*2,bProfi[1]+bProfi[3]/2);
	if (thermocouple.readCelsius() < 4000) {
		tft.println(String(tempOld) + " C");
	} else {
		tft.println("???");
	}

	long previousMillis = 0;
	long interval = 1000;

	while (true) {
		unsigned long currentMillis = millis();

		if(currentMillis - previousMillis > interval) {
   			previousMillis = currentMillis;

   			if (isHeatOn) {
   				tft.fillCircle(bCusto[0]+10,bCusto[1]+10,5,RED);
   			} else {
   				tft.fillCircle(bCusto[0]+10,bCusto[1]+10,5,GREEN);
   			}

   			int tempNew = thermocouple.readCelsius();
   			if (tempOld != tempNew) {
	   			tft.fillRect(tft.width()/2+border+5*6*2,bProfi[1]+bProfi[3]/2, 5*7*2,8*2,WHITE);
	   			tft.setCursor(tft.width()/2+border+5*6*2,bProfi[1]+bProfi[3]/2);
	   			tft.setTextSize(2);
	   			if (thermocouple.readCelsius() < 4000) {
					tft.println(String(tempNew) + " C");
				} else {
					tft.println("???");
				}
				tempOld = tempNew;
			}
   		}

		TSPoint p = ts.getPoint();
		if (p.z > ts.pressureThreshhold) {
			p.x = map(p.x, TS_MINY, TS_MAXY, 0, tft.height());
			p.y = map(p.y, TS_MINX, TS_MAXX, 0, tft.width());
			int y = tft.height() - p.x;
			int x = p.y;

			if (x>bStart[0]&&x<bStart[0]+bStart[2] && y>bStart[1]&&y<bStart[1]+bStart[3]) {
				tft.fillScreen(YELLOW);
				tft.setTextSize(2);
				tft.setCursor(tft.width()/2-19*11*.5,tft.height()/2-8);
				tft.println("NOT IMPLEMENTED YET");
				delay(1000);
				mainWindow();
			} else if (x>bProfi[0]&&x<bProfi[0]+bProfi[2] && y>bProfi[1]&&y<bProfi[1]+bProfi[3]) {
				profiles();
				// mainWindow();
			} else if (x>bCusto[0]&&x<bCusto[0]+bCusto[2] && y>bCusto[1]&&y<bCusto[1]+bCusto[3]) {
				custom();
			} else if (x>bResta[0]&&x<bResta[0]+bResta[2] && y>bResta[1]&&y<bResta[1]+bResta[3]) {
      			heatOff();
      			asm volatile ("  jmp 0"); // TODO: Better reset
      		}
      	}
      }
  }

// Graph (Profile and actual overlay)
// Emergency stop with "Are you sure?" screen

void run() {
	// TODO: Initial window setup
  	tft.fillScreen(RED);

	// TODO: Read profile
	//       and sort it by time
  	double profile[][2] = PROFILE1;
  	int len = 6;

  	long timeStart = millis();

  	for (int i = 0; i<len-1; i++) {
  		double time1 = profile[i][0];
  		double temp1 = profile[i][1];

  		double time2 = profile[i+1][0];
  		double temp2 = profile[i+1][1];

  		double m = (temp2-temp1)/(time2-time1);
  		double b  = temp1 - m*time1;

		double t = double(millis()-timeStart)/1000; // seconds;
		double initial = t;

		while (t<time2) {
			double interval = 0.5; // seconds
			

			if (t-initial > interval) {
				double tempSet = t*m+b;

				double temp = thermocouple.readCelsius();
				if (temp < tempSet) {
					heatOn();
				} else {
					heatOff();
				}
				// TODO: Update graph
				initial = t;
			}
			// TODO: Check for button presses
			t = double(millis()-timeStart)/1000;
		}
	}

	// TODO: Done window/message
	while (true) {
		TSPoint p = ts.getPoint();
		if (p.z > ts.pressureThreshhold) {
			mainWindow();
		}
	}
}

void addProfile(int num) {
}

void readProfileName(int num) {
	int current = 0;
	int pos = 2;
}

void readProfile(int num) {
	int current = 0;
	int pos = 2;
}

void deleteProfile(int num) {
}

void clearProfiles() {
	for (int i = 0; i<256; i++) {
		EEPROM.write(i,255);
	}
}

void test5() {
	tft.fillScreen(WHITE);

	clearProfiles();

	String text = "abc";
	int newText[text.length()];
	for (int i=0; i<text.length(); i++) {
		newText[i] = text[i];
	}

	tft.setCursor(0,0);
	EEPROM_put(0,3,1,newText);

	int *intOut = EEPROM_get(0,3,1);
	String textOut = "";
	for (int i=0; i<text.length(); i++) {
		Serial.println(char(intOut[i]));
		textOut += char(intOut[i]);
	}

	tft.println(textOut);
}

void profiles() {
	// test5();
	int graph [4] = {border*3,border*2,tft.width()-border*5,tft.height()*.6};

	tft.fillScreen(WHITE);

	tft.setCursor(graph[0]+3,graph[1]-8);
	tft.setTextSize(1);
	tft.println(PROFILE1N);

	double profile[][2] = PROFILE1;
	graphProfile(graph[0],graph[1]+graph[3]*.2,graph[2],graph[3]*.8,profile,true);
	tft.drawRect(graph[0],graph[1],graph[2],graph[3],BLACK);


	int bWidth = (tft.width()-border*3)*.5;  //(320-45)*.5 = 137.5
	int startH = (tft.height()-border*3)*.5; //(240-45)*.5 = 97.5
	int otherH = (startH-border)*.5;         // (97-15)*.5 = 41

	int bChang [4] = {border,         startH+otherH+border*3,bWidth,otherH};
	int bAddPr [4] = {border*2+bWidth,startH+otherH+border*3,bWidth,otherH};

	tft.setTextSize(tSize);

	tft.drawRoundRect(bChang[0],bChang[1],bChang[2],bChang[3],7,BLACK);
	tft.setCursor(bChang[0]+(bChang[2]-tSize*7*5)*.5,bChang[1]+(bChang[3]-tSize*8)*.5);
	tft.println("Change");
	// Restart Button
	tft.drawRoundRect(bAddPr[0],bAddPr[1],bAddPr[2],bAddPr[3],7,BLACK);
	tft.setCursor(bAddPr[0]+(bAddPr[2]-tSize*4*5)*.5,bAddPr[1]+(bAddPr[3]-tSize*8)*.5);
	tft.println("Add");

	while (true) {
		TSPoint p = ts.getPoint();
		if (p.z>ts.pressureThreshhold) {
			p.x = map(p.x, TS_MINY, TS_MAXY, 0, tft.height());
			p.y = map(p.y, TS_MINX, TS_MAXX, 0, tft.width());
			int y = tft.height() - p.x;
			int x = p.y;

			if (x>bChang[0]&&x<bChang[0]+bChang[2] && y>bChang[1]&&y<bChang[1]+bChang[3]) {
				tft.fillScreen(BLUE);
				tft.setTextSize(2);
				tft.setCursor(tft.width()/2-19*11*.5,tft.height()/2-8);
				tft.println("NOT IMPLEMENTED YET");
				delay(1000);
				profiles();
			} else if (x>bAddPr[0]&&x<bAddPr[0]+bAddPr[2] && y>bAddPr[1]&&y<bAddPr[1]+bAddPr[3]) {
      			tft.fillScreen(GREEN);
				tft.setTextSize(2);
				tft.setCursor(tft.width()/2-19*11*.5,tft.height()/2-8);
				tft.println("NOT IMPLEMENTED YET");
				delay(1000);
				profiles();
      		} else if (y<graph[1]+graph[3]) {
      			mainWindow();
      		}
		}
	}
}

#define FRAME_X 210
#define FRAME_Y 180
#define FRAME_W 100
#define FRAME_H 50

#define REDBUTTON_X FRAME_X
#define REDBUTTON_Y FRAME_Y
#define REDBUTTON_W (FRAME_W/2)
#define REDBUTTON_H FRAME_H

#define GREENBUTTON_X (REDBUTTON_X + REDBUTTON_W)
#define GREENBUTTON_Y FRAME_Y
#define GREENBUTTON_W (FRAME_W/2)
#define GREENBUTTON_H FRAME_H

void drawFrame() {
	tft.drawRect(FRAME_X, FRAME_Y, FRAME_W, FRAME_H, BLACK);
}

void redBtn() { 
	tft.fillRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, RED);
	tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, WHITE);
	drawFrame();
	tft.setCursor(GREENBUTTON_X + 6 , GREENBUTTON_Y + (GREENBUTTON_H/2));
	tft.setTextColor(BLACK);
	tft.setTextSize(2);
	tft.println("ON");
	heatOn();
}

void greenBtn() {
	tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, GREEN);
	tft.fillRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, WHITE);
	drawFrame();
	tft.setCursor(REDBUTTON_X + 6 , REDBUTTON_Y + (REDBUTTON_H/2));
	tft.setTextColor(BLACK);
	tft.setTextSize(2);
	tft.println("OFF");
	heatOff();
}

void custom() {
	tft.fillScreen(WHITE);

	if (isHeatOn) {
		redBtn();
	} else {
		greenBtn();
	}

	tft.setCursor(FRAME_X,FRAME_Y-30);
	tft.setTextSize(2);
	tft.println("Temp:");
	int tempOld = thermocouple.readCelsius();
	tft.setCursor(FRAME_X+5*6*2,FRAME_Y-30);
	if (thermocouple.readCelsius() < 4000) {
		tft.println(String(tempOld) + " C");
	} else {
		tft.println("???");
	}

	long previousMillis = 0;
	long interval = 1000;

	while (true) {
		unsigned long currentMillis = millis();

		if(currentMillis - previousMillis > interval) {
   			previousMillis = currentMillis;
   			int tempNew = thermocouple.readCelsius();
   			if (tempOld != tempNew) {
	   			tft.fillRect(FRAME_X+5*6*2,FRAME_Y-30, 5*7*2,8*2,WHITE);
	   			tft.setCursor(FRAME_X+5*6*2,FRAME_Y-30);
	   			tft.setTextSize(2);
	   			if (thermocouple.readCelsius() < 4000) {
					tft.println(String(tempNew) + " C");
				} else {
					tft.println("???");
				}
				tempOld = tempNew;
			}
		}

		TSPoint p = ts.getPoint();

		if (p.z > 10 && p.z < 1000)
		{   
			p.x = map(p.x, TS_MINY, TS_MAXY, 0, tft.height());
			p.y = map(p.y, TS_MINX, TS_MAXX, 0, tft.width());
			int y = tft.height() - p.x;
			int x = p.y;

			if((x > REDBUTTON_X) && (x < (REDBUTTON_X + REDBUTTON_W))) {
				if ((y > REDBUTTON_Y) && (y <= (REDBUTTON_Y + REDBUTTON_H))) {
					Serial.println("Red btn hit"); 
					redBtn();
				}
			} else if((x > GREENBUTTON_X) && (x < (GREENBUTTON_X + GREENBUTTON_W))) {
				if ((y > GREENBUTTON_Y) && (y <= (GREENBUTTON_Y + GREENBUTTON_H))) {
					Serial.println("Green btn hit"); 
					greenBtn();
				}
			} else if ((x > 0) && (x < (tft.width()/2))) {
				mainWindow();
			}
		}  
	}
}
