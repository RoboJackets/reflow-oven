#include "windows.h"
#include "setup.h"

#define border 10
#define bPress 0x0760

void mainWindow() {
	tft.fillScreen(cBACK);

	// Display Buttons
	int bWidth = (tft.width()-border*3)*.5;
	int otherH = (tft.height()-5*border)/4;
	int startH = 2*otherH+border;

	Button bStart    (border,         border,                bWidth,startH,"START"  );
	Button bProfiles (border,         startH+border*2,       bWidth,otherH,"Profile");
	Button bCustom   (border,         startH+otherH+border*3,bWidth,otherH,"Custom" );
	Button bSettings (border*2+bWidth,startH+otherH+border*3,bWidth,otherH,"Settings");

	bStart.view();
	bProfiles.view();
	bCustom.view();
	bSettings.view();

	// Display Profile Graph
	int graph [4] = {border*3.5+bWidth,border*2+tSize*8,bWidth-border*1.8,startH-tSize*8-border};

	tft.drawRect(graph[0],graph[1],graph[2],graph[3],cFRONT);

	tft.setCursor(graph[0]-7,border);
	tft.println("Profile Info");

	tft.setCursor(graph[0]+3,graph[1]-8);
	tft.setTextSize(1);
	tft.println(profile.Name);

	graphProfile(graph[0],graph[1],graph[2],graph[3]);

	// Display Temperature
	viewTemp(tft.width()/2+border,startH+border*2+otherH/2);
	int tempOld = getTemp();


	long previousMillis = 0; // Time of last update
	long interval = 1000; // Update interval in ms

	while (true) {
		// Update for heat indicator & temperature
		if(millis() - previousMillis > interval) {
   			previousMillis = millis();

   			// Heat indicator
   			if (isHeatOn!=0) {
   				tft.fillCircle(border+10,startH+otherH+border*3+10,5,RED);
   			} else {
   				tft.fillCircle(border+10,startH+otherH+border*3+10,5,GREEN);
   			}

   			// Update temperature display
   			int tempNew = getTemp();
   			if (tempOld != tempNew) {
	   			viewTemp(tft.width()/2+border,startH+border*2+otherH/2);
				tempOld = tempNew;
			}
   		}

   		// Check for button presses
		if (ts.touched()) {
			TS_Point p = ts.getPoint();

			if (bStart.isPressed(p)) {
				bStart.fill(bPress);
				start(); return;
			} else if (bProfiles.isPressed(p)) {
				bProfiles.fill(bPress);
				// profiles(); return;
          return;
			} else if (bCustom.isPressed(p)) {
				bCustom.fill(bPress);
				custom(); return;
			} else if (bSettings.isPressed(p)) {
				bSettings.fill(bPress);
      			settings(); return;
      		}
      	}
    }
}

void settings() {
	tft.fillScreen(cBACK);

	// Display buttons
	int bWidth = (tft.width()-border*3)*.5;
	int otherH = (tft.height()-5*border)/4;
	int startH = 2*otherH+border;

	Button bRestart	(border*2+bWidth,startH+otherH+border*3,bWidth,otherH,"Restart");
	Button bBack	(border,border,bWidth,otherH,"Back");
	Button bCMode	(bWidth+border*2,startH+border*2,bWidth,otherH,"Color Mode");

	bRestart.fill(RED);
	bBack.view();
	bCMode.view();

	long previousMillis = 0; // Time of last update
	long interval = 1000; // Update interval in ms

	while (true) {
		unsigned long currentMillis = millis();

		// Check for button presses
		if (ts.touched()) {
			TS_Point p = ts.getPoint();

			if (bRestart.isPressed(p)) {
				bRestart.fill(bPress);
				heatOn(0);
    			asm volatile ("  jmp 0");
			} else if (bBack.isPressed(p)) {
				bBack.fill(bPress);
				return;
			} else if (bCMode.isPressed(p)) {
				bCMode.fill(bPress);
				uint16_t old = cFRONT;
				cFRONT = cBACK;
				cBACK = old;
				settings(); return;
			}
		}
	}
}

void start() {
	tft.fillScreen(cBACK);

	// Display Buttons
	int bWidth1 = 100;
	int bHeight1 = 30;

	Button bYes	((tft.width()/2  -bWidth1)/2, (tft.height()-bHeight1)*.75, bWidth1, bHeight1, "YES");
	Button bNo	((tft.width()*1.5-bWidth1)/2, (tft.height()-bHeight1)*.75, bWidth1, bHeight1, "NO");

	bYes.view();
	bNo.view();

	// Display start checks and warnings
	tft.setTextSize(1);
	tft.setCursor(bYes.x,(tft.height()-bYes.y)/2);    tft.println("Have you done all of the following?");
	tft.setCursor(bYes.x,(tft.height()-bYes.y)/2+10); tft.println("-----------------------------------");
	tft.setCursor(bYes.x,(tft.height()-bYes.y)/2+20); tft.println("Connected temp probe, power, and heat cables?");
	tft.setCursor(bYes.x,(tft.height()-bYes.y)/2+30); tft.println("Inserted board with components?");
	tft.setCursor(bYes.x,(tft.height()-bYes.y)/2+40); tft.println("Temperature below " + String(profile.MinStartTemp) + "?");
	tft.setCursor(bYes.x,(tft.height()-bYes.y)/2+50); tft.println("...(Add more)...");

	while (true) {
		// Check for button presses
		if (ts.touched()) {
			TS_Point p = ts.getPoint();

			if (bYes.isPressed(p)) {
				if (getTemp() < profile.MinStartTemp) {
					bYes.fill(bPress);
					run(); return;
				} else {
					// Display run error
					bYes.fill(RED);
					delay(500);
					bYes.fill(cBACK);
					tft.setCursor(bYes.x,bYes.y+bYes.h+border);
					tft.setTextSize(2);
					tft.setTextColor(RED);
					if (!(getTemp() < 4000)) {
						tft.println("TEMP NOT CONNECTED");
					} else {
						tft.println("ABOVE MIN START TEMP");
					}

					tft.setTextColor(cFRONT);
					delay(1000);
					tft.fillRect(bYes.x,bYes.y+bYes.h+border,26*10,16,cBACK);

					// run(); // Remove when not testing
					return;
				}
			} else if (bNo.isPressed(p)) {
				bNo.fill(bPress);
				return;
			}
      	}
    }
}

void run() {
	tft.fillScreen(cBACK);

	// Find max time/temp for profile
	int maxTemp = 0;
	int maxTime = 0;
	for (int i = 0; i<profile.Length; i++) {
		if (maxTemp<profile.Phases[i].ExitTemperatureC) {
			maxTemp = profile.Phases[i].ExitTemperatureC;
		}
		maxTime += profile.Phases[i].TargetDurationS;
	}

	// Display buttons
	int bWidth = (tft.width()-border*3)*.5;
	int otherH = (tft.height()-5*border)/4;
	int startH = 2*otherH+border;

	Button bStop (border*2+bWidth, startH+otherH+border*3,bWidth,otherH,"STOP");

	bStop.view();

	// View graph
	int graph [4] = {border*3,border*2,tft.width()-border*5,tft.height()*.6};

	tft.setCursor(graph[0]+3,graph[1]-8);
	tft.setTextSize(1);
	tft.println(profile.Name);

	tft.drawRect(graph[0],graph[1],graph[2],graph[3],cFRONT);


	double interval = 1; // Update interval in s

	double timeStart = millis();


	// Pre-heat stage
	heatOn(3);
	double t = double(millis()-timeStart)/1000;
	int sumTime = 0;
	// double initial = t;

	double errLast = 0;

	// Cycle through other stages
	for (int i = 0; i<profile.Length; i++) {

		//Print phase name
		tft.setTextSize(1);
		tft.fillRect(border,tft.height()-border-8, 7*10,8,cBACK);
		tft.setCursor(border,tft.height()-border-8);
		tft.println(profile.Phases[i].Name);

		// Calculate goal temp eqn
		double time1 = sumTime;
		double temp1;
		if (i!=0) {
			temp1 = profile.Phases[i-1].ExitTemperatureC;

			sumTime += profile.Phases[i].TargetDurationS;
		} else {
			// time1 = -1;
			temp1 = 0;
		}

		double time2 = sumTime;
		double temp2 = profile.Phases[i].ExitTemperatureC;

		double m = (temp2-temp1)/(time2-time1);
  		double b  = temp1 - m*time1;

  		int x1 = map(time1,0,maxTime,graph[0],graph[0]+graph[2]);
  		int x2 = map(time2,0,maxTime,graph[0],graph[0]+graph[2]);

  		int y1 = map(temp1,0,maxTemp,0,graph[3]*.8);
  		int y2 = map(temp2,0,maxTemp,0,graph[3]*.8);

  		if (i!=0) {
  			tft.drawLine(x1,graph[1]+graph[3]-y1,x2,graph[1]+graph[3]-y2,cFRONT);
  		}

		viewTemp(border,startH+otherH+border*3);
		int tempOld = getTemp();


		double stopTime = 0;
		double initial = t;

		// while (t<time2 || (i==0 && getTemp()<temp2)) {
		while (t<time2 || i==0) {
			// Check for button update
			if (ts.touched()) {
				TS_Point p = ts.getPoint();

				if (bStop.isPressed(p)) {
					if (stopTime > 0 && t-stopTime > .2){
						bStop.fill(bPress);
						heatOn(0);
						return;
					} else {
						stopTime = t;
						bStop.text = "Confirm?";
						bStop.fill(bPress);
						delay(200);
						bStop.fill(RED);
					}
				}
			}

			if (stopTime > 0 && t-stopTime > 4) {
				stopTime = 0;
				bStop.text = "STOP";
				bStop.fill(bPress);
				delay(200);
				bStop.fill(cBACK);
			}

			if (t-initial > interval) {
				double tempSet = m*t+b;
				if (i==0) {
					tempSet = temp2;
				}
				double temp = getTemp();

				double error = temp - tempSet;
				double dErr = (errLast-error)/interval;

				if (temp-dErr*4-6 < tempSet) {
					heatOn(3);
				} else {
					heatOn(0);
				}

				// Force heatup for reflow
				if (i==3 || (i==2 && t>time2-20) || i==0){
					heatOn(3);
				}

				// Force heatoff for cooldown
				if ((i==4 && t>time1) || i==5) {
					heatOn(0);
				}

				// View current & goal temp
				if (tempOld != int(temp)) {
					viewTemp(border,startH+otherH+border*3);
					tempOld = temp;
				}

				tft.setCursor(border+24,startH+otherH+border*4+12);
				tft.setTextSize(1);
				tft.print("Goal: ");
				// tft.fillRect(border+5*6*2,tft.height()-border-8, 5*7,8,cBACK);
				tft.fillRect(border+4*7+24,startH+otherH+border*4+12,4*8, 10,cBACK);
				// tft.setCursor(border+5*6*2,tft.height()-border-8);
				tft.println(String(int(tempSet)) + " C");


				// Update temperature graph
				int timeX = map(t,0,maxTime,graph[0],graph[0]+graph[2]);
				tft.drawPixel(timeX,graph[1]+graph[3],RED);
				tft.drawPixel(timeX,graph[1]+graph[3]-2,RED);

				// TODO: Better connection for graph
				if (getTemp() < 4000 || true) {
					int tempY = map(temp,0,maxTemp,0,graph[3]*.8);
					tempY = graph[1]+graph[3] - tempY;
					tft.drawPixel(timeX,tempY,RED);
				}

				// For heating up stage
				if (i==0) {
					tft.drawPixel(timeX,graph[1]+graph[3]-map(profile.Phases[0].ExitTemperatureC,0,maxTemp,0,graph[3]*.8),LIGHTGREY);
					if (temp>profile.Phases[0].ExitTemperatureC-6) {
						break;
					}
				}

				errLast = error;
				initial = t;
			}
			t = double(millis()-timeStart)/1000;
			if (i==0) {
				sumTime = t;
			}
		}
		if (profile.Phases[i].AlarmOnExit) {
			tone(speaker,60,1000);
		}
	}

	heatOn(0);
	bStop.text = "Done";
	bStop.fill(bPress);
	bStop.fill(cBACK);

	while (true) {
		if (ts.touched()) {
			TS_Point p = ts.getPoint();

			if (bStop.isPressed(p)) {
				bStop.fill(bPress);
				return;
			}
		}
	}
}

//void profiles() {
//	int otherH = (tft.height()-5*border)/4;
//
//	Button bProfile1 (border,border         ,tft.width()-border*2,otherH,profile1.Name);
//	Button bProfile2 (border,otherH+border*2,tft.width()-border*2,otherH,profile2.Name);
//
//	// Change ReflowProfile profile to selected profile
//	while (true) {
//		TS_Point p = ts.getPoint();
//		if (p.z > ts.pressureThreshhold) {
//			if (bProfile1.isPressed(p)) {
//				profile = profile1;
//				return;
//			} else if (bProfile2.isPressed(p)) {
//				profile = profile2;
//				return;
//			}
//      	}
//	}
//}

void custom() {
	tft.fillScreen(cBACK);

	Button bBack	(border,border,(tft.width()-border*3)*.5,tft.height()/4-border*2,"Back");
	Button bSwitchD	(210, 180, 100, 50, "OFF");
	Button bSwitchU	(100, 180, 100, 50, "OFF");

	bBack.view();

	if (isHeatOn==1 || isHeatOn==3) {
		bSwitchU.text = "ON";
		bSwitchU.fill(RED);
	} else {
		bSwitchU.text = "OFF";
		bSwitchU.fill(GREEN);
	}

	if (isHeatOn==2 || isHeatOn==3) {
		bSwitchD.text = "ON";
		bSwitchD.fill(RED);
	} else {
		bSwitchD.text = "OFF";
		bSwitchD.fill(GREEN);
	}

	tft.setTextSize(1);
	tft.setCursor(bSwitchU.x,bSwitchU.y-10);
	tft.println("Heat Upper");
	tft.setCursor(bSwitchD.x,bSwitchD.y-10);
	tft.println("Heat Bottom");

	viewTemp(bSwitchU.x,bSwitchU.y-60);
	int tempOld = getTemp();

	long previousMillis = 0;
	long interval = 1000;

	while (true) {
		unsigned long currentMillis = millis();

		// Update temperature
		if(currentMillis - previousMillis > interval) {
   			previousMillis = currentMillis;
   			int tempNew = getTemp();
   			if (tempOld != tempNew) {
				viewTemp(bSwitchU.x,bSwitchU.y-60);
				tempOld = tempNew;
			}
		}

		// Check for button presses
		if (ts.touched()) {
			TS_Point p = ts.getPoint();

			if (bSwitchU.isPressed(p)) {
				if (isHeatOn==0 || isHeatOn==2) {
					bSwitchU.text = "ON";
					bSwitchU.fill(RED);
                    heatOn(isHeatOn+1);
				} else {
					bSwitchU.text = "OFF";
					bSwitchU.fill(GREEN);
					heatOn(isHeatOn-1);
				}
				delay(500);
			} else if (bSwitchD.isPressed(p)) {
				if (isHeatOn==0 || isHeatOn==1) {
					bSwitchD.text = "ON";
					bSwitchD.fill(RED);
					heatOn(isHeatOn+2);
				} else {
					bSwitchD.text = "OFF";
					bSwitchD.fill(GREEN);
					heatOn(isHeatOn-2);
				}
				delay(500);
			} else if (bBack.isPressed(p)) {
				bBack.fill(bPress);
				delay(200);
				return;
			}
		}
	}
}

void graphProfile(int x, int y, int w, int h) {
	// Find max time and temp
	int maxTemp = 0;
	int maxTime = 0;
	for (int i = 0; i<profile.Length;i++) {
		if (maxTemp<profile.Phases[i].ExitTemperatureC) {
			maxTemp = profile.Phases[i].ExitTemperatureC;
		}
		maxTime += profile.Phases[i].TargetDurationS;
	}

	int currentTime = 0;
	// Loop through profile phases
	for (int i = 0; i<profile.Length;i++) {
		int x1 = map(long(currentTime),0,long(maxTime),0,long(w));
		currentTime += profile.Phases[i].TargetDurationS;
		int x2 = map(long(currentTime),0,long(maxTime),0,long(w));

		int y1 = 25;
		if (i!=0) {
			y1 = map(long(profile.Phases[i-1].ExitTemperatureC),0,long(maxTemp),0,long(h));
		}
		int y2 = map(long(profile.Phases[i].ExitTemperatureC),0,long(maxTemp),0,long(h));

		// Draw phase line & vertical/horizontal lines
		tft.drawLine(x+x1,y+h-y1,x+x2-1,y+h-y2,cFRONT);

		tft.drawLine(x+x1,y+h-3,x+x1,y+h+3,cFRONT);
		tft.drawLine(x-3,y+h-y1,x+3,y+h-y1,cFRONT);

		tft.setTextColor(cFRONT);

		if (i==0) {
			tft.setTextSize(1);

			String time1 = String(x1);
			tft.setCursor(x+x1-time1.length()*2.5,y+h+5);
			tft.println(time1);

			String temp1 = String(y1);
			tft.setCursor(x-3-temp1.length()*5-3,y+h-y1-8/2+2);
			tft.println(temp1);
		}
	}

	String time1 = String(maxTime);
	tft.setCursor(x+w-time1.length()*2.5,y+h+5);
	tft.println(time1);

	String temp1 = String(maxTemp);
	tft.setCursor(x-3-temp1.length()*5-3,y-8/2+2);
	tft.println(temp1);
}

void viewTemp(int x, int y) {
	// Update temp at x,y with size 2
	tft.setCursor(x,y);
	tft.setTextSize(2);
	tft.println("Temp:");
	tft.fillRect(x+5*6*2,y, 5*7*2,8*2,cBACK);
	tft.setCursor(x+5*6*2,y);
	if (getTemp() < 4000) {
		tft.println(String(int(getTemp())) + " C");
	} else {
		tft.println("???");
	}
}
