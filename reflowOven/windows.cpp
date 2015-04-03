#include "windows.h"

// Start button
// View/add Profiles Button
// Current profile info
// Restart button

#define border 10

void start() {
	mainWindow();
}

void mainWindow() {
	tft.fillScreen(WHITE);

	int bWidth = (tft.width()-border*3)*.5;  //(320-45)*.5 = 137.5
	int startH = (tft.height()-border*3)*.5; //(240-45)*.5 = 97.5
	int otherH = (startH-border)*.5;         // (97-15)*.5 = 41

	Button bStart    (border,         border,                bWidth,startH,"START"  );
	Button bProfiles (border,         startH+border*2,       bWidth,otherH,"Profile");
	Button bCustom   (border,         startH+otherH+border*3,bWidth,otherH,"Custom" );
	Button bRestart  (border*2+bWidth,startH+otherH+border*3,bWidth,otherH,"Restart");

	bStart.view();
	bProfiles.view();
	bCustom.view();
	bRestart.view();

	int graph [4] = {border*3.5+bWidth,border*2+tSize*8,bWidth-border*1.8,startH-tSize*8-border};

	tft.drawRect(graph[0],graph[1],graph[2],graph[3],BLACK);

	tft.setCursor(graph[0]-7,border);
	tft.println("Profile Info");

	tft.setCursor(graph[0]+3,graph[1]-8);
	tft.setTextSize(1);
	tft.println(PROFILE1N);

	double profile[][2] = PROFILE1;
	graphProfile(graph[0],graph[1],graph[2],graph[3],profile,false);

	viewTemp(tft.width()/2+border,startH+border*2+otherH/2);
	int tempOld = thermocouple.readCelsius();

	long previousMillis = 0;
	long interval = 1000;

	while (true) {
		unsigned long currentMillis = millis();

		if(currentMillis - previousMillis > interval) {
   			previousMillis = currentMillis;

   			if (isHeatOn) {
   				tft.fillCircle(border+10,startH+otherH+border*3+10,5,RED);
   			} else {
   				tft.fillCircle(border+10,startH+otherH+border*3+10,5,GREEN);
   			}

   			int tempNew = thermocouple.readCelsius();
   			if (tempOld != tempNew) {
	   			viewTemp(tft.width()/2+border,startH+border*2+otherH/2);
				tempOld = tempNew;
			}
   		}

		TSPoint p = ts.getPoint();
		if (p.z > ts.pressureThreshhold) {
			if (bStart.isPressed(p)) {
				tft.fillScreen(YELLOW);
				tft.setTextSize(2);
				tft.setCursor(tft.width()/2-19*11*.5,tft.height()/2-8);
				tft.println("NOT IMPLEMENTED YET");
				delay(1000);
				mainWindow();
			} else if (bProfiles.isPressed(p)) {
				profiles();
			} else if (bCustom.isPressed(p)) {
				custom();
			} else if (bRestart.isPressed(p)) {
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

	Button bChange (border,         startH+otherH+border*3,bWidth,otherH,"Change");
	Button bAdd    (border*2+bWidth,startH+otherH+border*3,bWidth,otherH,"Add");

	bChange.view();
	bAdd.view();

	while (true) {
		TSPoint p = ts.getPoint();
		int y = tft.height()-map(p.x, TS_MINX, TS_MAXX, 0, tft.height());
		if (p.z>ts.pressureThreshhold) {
			if (bChange.isPressed(p)) {
				tft.fillScreen(BLUE);
				tft.setTextSize(2);
				tft.setCursor(tft.width()/2-19*11*.5,tft.height()/2-8);
				tft.println("NOT IMPLEMENTED YET");
				delay(1000);
				profiles();
			} else if (bAdd.isPressed(p)) {
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

// #define SWITCH_X 210
// #define SWITCH_Y 180
// #define SWITCH_W 100
// #define SWITCH_H 50

// void redBtn() { 
// 	tft.fillRect(SWITCH_X, SWITCH_Y, SWITCH_W/2, SWITCH_H, RED);
// 	tft.fillRect(SWITCH_X+SWITCH_W/2, SWITCH_Y, SWITCH_W/2, SWITCH_H, WHITE);
// 	tft.drawRect(SWITCH_X, SWITCH_Y, SWITCH_W, SWITCH_H, BLACK);
// 	tft.setCursor(SWITCH_X +SWITCH_W/2 + 6 , SWITCH_Y + (SWITCH_H/2));
// 	tft.setTextColor(BLACK);
// 	tft.setTextSize(2);
// 	tft.println("ON");
// 	heatOn();
// }

// void greenBtn() {
// 	tft.fillRect(SWITCH_X+SWITCH_W/2, SWITCH_Y, SWITCH_W/2, SWITCH_H, GREEN);
// 	tft.fillRect(SWITCH_X, SWITCH_Y, SWITCH_W/2, SWITCH_H, WHITE);
// 	tft.drawRect(SWITCH_X, SWITCH_Y, SWITCH_W, SWITCH_H, BLACK);
// 	tft.setCursor(SWITCH_X + 6 , SWITCH_Y + (SWITCH_H/2));
// 	tft.setTextColor(BLACK);
// 	tft.setTextSize(2);
// 	tft.println("OFF");
// 	heatOff();
// }

// void custom() {
// 	tft.fillScreen(WHITE);

// 	if (isHeatOn) {
// 		redBtn();
// 	} else {
// 		greenBtn();
// 	}

// 	viewTemp(SWITCH_X,SWITCH_Y-30);
// 	int tempOld = thermocouple.readCelsius();

// 	long previousMillis = 0;
// 	long interval = 1000;

// 	while (true) {
// 		unsigned long currentMillis = millis();

// 		if(currentMillis - previousMillis > interval) {
//    			previousMillis = currentMillis;
//    			int tempNew = thermocouple.readCelsius();
//    			if (tempOld != tempNew) {
// 				viewTemp(SWITCH_X,SWITCH_Y-30);
// 				tempOld = tempNew;
// 			}
// 		}

// 		TSPoint p = ts.getPoint();

// 		if (p.z > 10 && p.z < 1000)
// 		{   
// 			p.x = map(p.x, TS_MINY, TS_MAXY, 0, tft.height());
// 			p.y = map(p.y, TS_MINX, TS_MAXX, 0, tft.width());
// 			int y = tft.height() - p.x;
// 			int x = p.y;

// 			if ((y > SWITCH_Y) && (y <= (SWITCH_Y + SWITCH_H))) {
// 				if ((x > SWITCH_X) && (x < (SWITCH_X + SWITCH_W/2))) {
// 					redBtn();
// 				} else if ((x > SWITCH_X+SWITCH_W/2) && (x < (SWITCH_X + SWITCH_W))) {
// 					greenBtn();
// 				}
// 			}
// 			if ((x > 0) && (x < (tft.width()/2))) {
// 				mainWindow();
// 			}
// 		}  
// 	}
// }

void custom() {
	tft.fillScreen(WHITE);

	Button bSwitch(210, 180, 100, 50, "OFF");

	if (isHeatOn) {
		bSwitch.text = "ON";
		bSwitch.fill(RED);
	} else {
		bSwitch.text = "OFF";
		bSwitch.fill(GREEN);
	}

	viewTemp(bSwitch.x,bSwitch.y-30);
	int tempOld = thermocouple.readCelsius();

	long previousMillis = 0;
	long interval = 1000;

	while (true) {
		unsigned long currentMillis = millis();

		if(currentMillis - previousMillis > interval) {
   			previousMillis = currentMillis;
   			int tempNew = thermocouple.readCelsius();
   			if (tempOld != tempNew) {
				viewTemp(bSwitch.x,bSwitch.y-30);
				tempOld = tempNew;
			}
		}

		TSPoint p = ts.getPoint();
		int x = map(p.y, TS_MINX, TS_MAXX, 0, tft.width());

		if (p.z > 10 && p.z < 1000)
		{   
			if (bSwitch.isPressed(p)) {
				if (isHeatOn) {
					isHeatOn = false;
					bSwitch.text = "OFF";
					bSwitch.fill(GREEN);
				} else {
					isHeatOn = true;
					bSwitch.text = "ON";
					bSwitch.fill(RED);
				}
				delay(500);
			}
			if ((x > 0) && (x < (tft.width()/2))) {
				mainWindow();
			}
		}  
	}
}



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

void viewTemp(int x, int y) {
	tft.setCursor(x,y);
	tft.setTextSize(2);
	tft.println("Temp:");
	tft.fillRect(x+5*6*2,y, 5*7*2,8*2,WHITE);
	tft.setCursor(x+5*6*2,y);
	if (thermocouple.readCelsius() < 4000) {
		tft.println(String(int(thermocouple.readCelsius())) + " C");
	} else {
		tft.println("???");
	}
}