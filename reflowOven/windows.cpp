#include "windows.h"

#define border 10
#define bPress 0x0760


void mainWindow() {
	tft.fillScreen(cBACK);

	tft.setCursor(20,20);
	tft.setTextSize(1);
	tft.println(freeRam());

	if (freeRam()<250) {
		heatOn(0);
		asm volatile ("  jmp 0"); // TODO: Better reset
	}

	int bWidth = (tft.width()-border*3)*.5;
	int startH = (tft.height()-border*3)*.5;
	int otherH = (startH-border)*.5;

	Button bStart    (border,         border,                bWidth,startH,"START"  );
	Button bProfiles (border,         startH+border*2,       bWidth,otherH,"Profile");
	Button bCustom   (border,         startH+otherH+border*3,bWidth,otherH,"Custom" );
	Button bSettings (border*2+bWidth,startH+otherH+border*3,bWidth,otherH,"Settings");

	bStart.view();
	bProfiles.view();
	bCustom.view();
	bSettings.view();

	int graph [4] = {border*3.5+bWidth,border*2+tSize*8,bWidth-border*1.8,startH-tSize*8-border};

	tft.drawRect(graph[0],graph[1],graph[2],graph[3],cFRONT);

	tft.setCursor(graph[0]-7,border);
	tft.println("Profile Info");

	tft.setCursor(graph[0]+3,graph[1]-8);
	tft.setTextSize(1);
	tft.println(profile.name);
	
	graphProfile(graph[0],graph[1],graph[2],graph[3],profile,false);

	viewTemp(tft.width()/2+border,startH+border*2+otherH/2);
	int tempOld = getTemp();

	long previousMillis = 0;
	long interval = 1000;

	while (true) {
		unsigned long currentMillis = millis();

		if(currentMillis - previousMillis > interval) {
   			previousMillis = currentMillis;

   			if (isHeatOn!=0) {
   				tft.fillCircle(border+10,startH+otherH+border*3+10,5,RED);
   			} else {
   				tft.fillCircle(border+10,startH+otherH+border*3+10,5,GREEN);
   			}

   			int tempNew = getTemp();
   			if (tempOld != tempNew) {
	   			viewTemp(tft.width()/2+border,startH+border*2+otherH/2);
				tempOld = tempNew;
			}
   		}

		TSPoint p = ts.getPoint();
		if (p.z > ts.pressureThreshhold) {
			if (bStart.isPressed(p)) {
				bStart.fill(bPress);
				delay(100);	// why?
				start(); return;
			} else if (bProfiles.isPressed(p)) {
				bProfiles.fill(bPress);
				profiles(); return;
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

	int bWidth = (tft.width()-border*3)*.5;
	int startH = (tft.height()-border*3)*.5;
	int otherH = (startH-border)*.5;

	Button bRestart	(border*2+bWidth,startH+otherH+border*3,bWidth,otherH,"Restart");
	Button bBack	(border,border,bWidth,otherH,"Back");
	Button bCMode	(bWidth+border*2,startH+border*2,bWidth,otherH,"Color Mode");

	bRestart.fill(RED);
	bBack.view();
	bCMode.view();

	long previousMillis = 0;
	long interval = 1000;

	while (true) {
		unsigned long currentMillis = millis();

		TSPoint p = ts.getPoint();
		if (p.z > ts.pressureThreshhold) {
			if (bRestart.isPressed(p)) {
				bRestart.fill(bPress);
				heatOn(0);
    			asm volatile ("  jmp 0"); // TODO: Better reset
			} else if (bBack.isPressed(p)) {
				bBack.fill(bPress);
				delay(100);
				// mainWindow();
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
	int bWidth = 100;
	int bHeight = 30;

	Button bYes	((tft.width()/2  -bWidth)/2, (tft.height()-bHeight)*.75, bWidth, bHeight, "YES");
	Button bNo	((tft.width()*1.5-bWidth)/2, (tft.height()-bHeight)*.75, bWidth, bHeight, "NO");

	tft.fillScreen(cBACK);

	tft.setTextSize(1);
	tft.setCursor(bYes.x,(tft.height()-bYes.y)/2);
	tft.println("Have you done all of the following?");
	tft.setCursor(bYes.x,(tft.height()-bYes.y)/2+10);
	tft.println("-----------------------------------");
	tft.setCursor(bYes.x,(tft.height()-bYes.y)/2+20);
	tft.println("Connected temp probe, power, and heat cables?");
	tft.setCursor(bYes.x,(tft.height()-bYes.y)/2+30);
	tft.println("Inserted board with components?");
	tft.setCursor(bYes.x,(tft.height()-bYes.y)/2+40);
	tft.println("...(Add more)...");

	bYes.view();
	bNo.view();

	while (true) {
		TSPoint p = ts.getPoint();
		if (p.z > ts.pressureThreshhold) {
			if (bYes.isPressed(p)) {
				if (getTemp() < 4000) {
					bYes.fill(bPress);
					run();
				} else {
					bYes.fill(RED);
					delay(500);
					bYes.fill(cBACK);
					tft.setCursor(bYes.x,bYes.y+bYes.h+border);
					tft.setTextSize(2);
					tft.setTextColor(RED);
					tft.println("TEMP NOT CONNECTED");
					tft.setTextColor(cFRONT);
					delay(1000);
					tft.fillRect(bYes.x,bYes.y+bYes.h+border,22*10,16,cBACK);

					run(); // Remove when not testing
				}
			} else if (bNo.isPressed(p)) {
				bNo.fill(bPress);
				// mainWindow();
				return;
			}
      	}
    }
}

void run() {
	tft.fillScreen(cBACK);

  	int graph [4] = {border*3,border*2,tft.width()-border*5,tft.height()*.6};

	tft.setCursor(graph[0]+3,graph[1]-8);
	tft.setTextSize(1);
	tft.println(profile.name);

	graphProfile(graph[0],graph[1]+graph[3]*.2,graph[2],graph[3]*.8,profile,true);
	tft.drawRect(graph[0],graph[1],graph[2],graph[3],cFRONT);

	int bWidth = (tft.width()-border*3)*.5;
	int startH = (tft.height()-border*3)*.5;
	int otherH = (startH-border)*.5;

	Button bStop (border*2+bWidth, startH+otherH+border*3,bWidth,otherH,"STOP");

	bStop.view();

	double maxTemp = profile.values[0][1];
	double maxTime = profile.values[profile.length-1][0];
	for (int i = 1; i<profile.length-1; i++) {
		if (profile.values[i][1]>maxTemp) {
			maxTemp = profile.values[i][1];
		}
	}


  	long timeStart = millis();

  	for (int i = 0; i<profile.length-1; i++) {
  		double time1 = profile.values[i][0];
  		double temp1 = profile.values[i][1];

  		double time2 = profile.values[i+1][0];
  		double temp2 = profile.values[i+1][1];

  		double m = (temp2-temp1)/(time2-time1);
  		double b  = temp1 - m*time1;

		double t = double(millis()-timeStart)/1000; // seconds;
		double initial = t;

		viewTemp(border,startH+otherH+border*3);
		int tempOld = getTemp();

		double stopTime = 0;

		while (t<time2) {
			// double interval = 0.5; // seconds
			double interval = double(maxTime)/double(graph[2])*.5;

			TSPoint p = ts.getPoint();
			if (p.z > ts.pressureThreshhold) {
				if (bStop.isPressed(p)) {
					if (stopTime > 0 && t-stopTime > .2){
						bStop.fill(bPress);
						heatOn(0);
						// mainWindow();
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
				double tempSet = t*m+b;

				double temp = getTemp();
				if (tempOld != int(temp)) {
					viewTemp(border,startH+otherH+border*3);
					tempOld = temp;
				}

				tft.setCursor(border,tft.height()-border-18);
				tft.setTextSize(2);
				tft.println("Goal:");
				tft.fillRect(border+5*6*2,tft.height()-border-18, 5*7*2,8*2,cBACK);
				tft.setCursor(border+5*6*2,tft.height()-border-18);
				tft.println(String(int(tempSet)) + " C");

				if (temp < tempSet) {
					heatOn(3);
				} else {
					heatOn(0);
				}

				int timeX = map(t,0,maxTime,graph[0],graph[0]+graph[2]);
				tft.drawPixel(timeX,graph[1]+graph[3],RED);
				tft.drawPixel(timeX,graph[1]+graph[3]-2,RED);

				// TODO: Better connection for graph
				if (getTemp() < 4000 || true) {
					int tempY = map(temp,0,maxTemp,0,graph[3]*.8);
					tempY = graph[1]+graph[3] - tempY;
					tft.drawPixel(timeX,tempY,RED);
				}

				initial = t;
			}
			t = double(millis()-timeStart)/1000;
		}
	}

	heatOn(0);
	bStop.text = "Done";
	bStop.fill(bPress);
	bStop.fill(cBACK);

	while (true) {
		TSPoint p = ts.getPoint();
		if (p.z > ts.pressureThreshhold) {
			if (bStop.isPressed(p)) {
				bStop.fill(bPress);
				// mainWindow();
				return;
			}
		}
	}
}

void profiles() {
	
}

// void profiles() {
// 	int graph [4] = {border*3,border*2,tft.width()-border*5,tft.height()*.6};

// 	tft.fillScreen(cBACK);

// 	tft.setCursor(graph[0]+3,graph[1]-8);
// 	tft.setTextSize(1);
// 	tft.println(profile.name);

	
// 	graphProfile(graph[0],graph[1]+graph[3]*.2,graph[2],graph[3]*.8,profile,true);
// 	tft.drawRect(graph[0],graph[1],graph[2],graph[3],cFRONT);


// 	int bWidth = (tft.width()-border*3)*.5;
// 	int startH = (tft.height()-border*3)*.5;
// 	int otherH = (startH-border)*.5;

// 	Button bChange	(border,         startH+otherH+border*3,bWidth,otherH,"Change");
// 	Button bAdd		(border*2+bWidth,startH+otherH+border*3,bWidth,otherH,"Add");

// 	bChange.view();
// 	bAdd.view();

// 	while (true) {
// 		TSPoint p = ts.getPoint();
// 		int y = tft.height()-map(p.x, TS_MINX, TS_MAXX, 0, tft.height());
// 		if (p.z>ts.pressureThreshhold) {
// 			if (bChange.isPressed(p)) {
// 				bChange.fill(bPress);

// 				tft.fillScreen(BLUE);
// 				tft.setTextSize(2);
// 				tft.setCursor(tft.width()/2-19*11*.5,tft.height()/2-8);
// 				tft.println("NOT IMPLEMENTED YET");
// 				delay(1000);
// 				profiles(); return;
// 			} else if (bAdd.isPressed(p)) {
//       			bAdd.fill(bPress);

//       			addProfile(); return;

//     	   		// tft.fillScreen(GREEN);
// 				// tft.setTextSize(2);
// 				// tft.setCursor(tft.width()/2-19*11*.5,tft.height()/2-8);
// 				// tft.println("NOT IMPLEMENTED YET");
// 				// delay(1000);
// 				// profiles();
//       		} else if (y<graph[1]+graph[3]) {
//       			// mainWindow();
//       			return;
//       		}
// 		}
// 	}
// }

// char KB0[4][11] ={
//   {10 ,'1','2','3','4','5','6','7','8','9','0'},
//   {10 ,'Q','W','E','R','T','Y','U','I','O','P'},
//   {9  ,  'A','S','D','F','G','H','J','K','L'  },
//   {7  ,      'Z','X','C','V','B','N','M'      } //del
//   //            |       Space       | | Enter |
// };

// char KP[4][11] ={
//   {3, '1','2','3'},
//   {3, '4','5','6'},
//   {3, '7','8','9'},
//   {3, '<','0','>'}
// };

// void addProfile() {
// 	tft.fillScreen(cBACK);
// 	keyboard(border,border+tft.height()*.2,tft.width()-border,tft.height()*.8-border, 4,10,KB0);

// 	String text = "";
// 	while (true) {
// 		TSPoint p = ts.getPoint();

// 		char temp = (keyboardB(border,border+tft.height()*.2,tft.width()-border,tft.height()*.8-border,4,10,KB0,p));
// 		if (temp!=5 && temp!=0) {

// 			if (temp==1) {
// 				text = text.substring(0,text.length()-1);
// 			} else if (temp==2) {
// 				break;
// 		} else {
// 				text += temp;
// 			}

// 			tft.fillRect(border,border,(text.length()+1)*12,16,cBACK);
// 			tft.setCursor(border,border);
// 			tft.print(text);
// 		}
// 	}

// 	profile.name = text;

// 	tft.fillScreen(cBACK);
// 	keyboard(border,border,tft.width()/2,tft.height()-border, 4,3,KP);
// 	tft.setCursor(border+(tft.width()/2-80)*.5,tft.height()*.85);
// 	tft.print("Confirm");

// 	tft.setCursor(tft.width()*.75-5.5*10,border);
// 	tft.print("Time  Temp");

// 	int count = 0;
// 	int current = 0;
// 	int vals[20];
// 	while (true) {
// 		TSPoint p = ts.getPoint();
// 		char temp = keyboardB(border,border,tft.width()/2,tft.height()-border, 4,3,KP,p);
// 		if (temp == ' ') {
// 			tft.setCursor(border+(tft.width()/2-80)*.5,tft.height()*.85);
// 			tft.print("Confirm");
// 			if (count%2==0 && current==0){
// 				break;
// 			}

// 		} else if (((temp>=48 && temp<=57) || temp=='<' || temp=='>')&&count<20) {
// 			int num = temp-48;
// 			int y3 = border+25+20*int(count/2);
// 			int x3;
// 			if (count%2==0) {
// 				x3 = tft.width()*.75-5.5*10;
// 			} else {
// 				x3 = tft.width()*.75-5.5*10+70;
// 			}
// 			tft.fillRect(x3,y3,10*4,16,cBACK);
// 			tft.setCursor(x3,y3);
// 			if (temp=='<' || current>=1000) {
// 				current = 0;
// 			} else if (temp=='>') {
// 				vals[count] = current;
// 				count++;
// 			} else {
// 				current = current*10 + num;
// 			}
// 			tft.print(current);
// 			if (temp=='>') {
// 				current = 0;
// 			}
// 		}
// 	}

// 	int valOut[count/2][2];
// 	for (int i=0; i<count; i++) {
// 		valOut[int(i/2)][i%2]=vals[i];
// 	}

// 	Profile profile2 = Profile(count/2,valOut,text);

// 	tft.fillScreen(cBACK);

// 	int graph [4] = {border*3,border*2,tft.width()-border*5,tft.height()*.6};

// 	tft.setCursor(graph[0],0);
// 	tft.println(profile2.name);

// 	graphProfile(graph[0],graph[1]+graph[3]*.2,graph[2],graph[3]*.8,profile2,true);
// 	tft.drawRect(graph[0],graph[1],graph[2],graph[3],cFRONT);

// 	profile = profile2;

// 	while (true) {
// 		TSPoint p = ts.getPoint();
// 		if (p.z>ts.pressureThreshhold) {
// 			// mainWindow();
// 			return;
// 		}
// 	}
// }

void custom() {
	tft.fillScreen(cBACK);

	// int profile1[][2] = PROFILE1;
	// Profile test = Profile(6,profile1,PROFILE1N);

	// tft.setCursor(0,0);
	// tft.setTextSize(1);

	// addProfile(test);
	// Profile test2 = readProfile(0);

	// tft.setCursor(0,0);
	// tft.println(test.name);
	// tft.println(test.length);
	// tft.println();
	// tft.println(test2.name);
	// tft.println(test2.length);

	// String out = "";
	// for (int i = 0; i < 6; i++) {
	// 	out += "{";
	// 	for (int j = 0; j<2; j++) {
	// 		out += String(test2.values[i][j])+",";
	// 	}
	// 	out += "}";
	// }
	// tft.println(out);

	Button bBack	(border,border,(tft.width()-border*3)*.5,tft.height()/4-border*2,"Back");
	Button bSwitchU	(210, 180, 100, 50, "OFF");
	Button bSwitchD	(100, 180, 100, 50, "OFF");

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
	// if (isHeatOn!=0) {
	// 	bSwitch.text = "ON";
	// 	bSwitch.fill(RED);
	// } else {
	// 	bSwitch.text = "OFF";
	// 	bSwitch.fill(GREEN);
	// }

	viewTemp(bSwitchD.x,bSwitchD.y-60);
	int tempOld = getTemp();

	long previousMillis = 0;
	long interval = 1000;

	while (true) {
		unsigned long currentMillis = millis();

		if(currentMillis - previousMillis > interval) {
   			previousMillis = currentMillis;
   			int tempNew = getTemp();
   			if (tempOld != tempNew) {
				viewTemp(bSwitchD.x,bSwitchD.y-60);
				tempOld = tempNew;
			}
		}

		TSPoint p = ts.getPoint();
		// int x = map(p.y, TS_MINX, TS_MAXX, 0, tft.width());

		if (p.z > 10 && p.z < 1000)
		{   
			if (bSwitchU.isPressed(p)) {
				// if (isHeatOn!=0) {
				// 	isHeatOn = 0;
				// 	bSwitch.text = "OFF";
				// 	bSwitch.fill(GREEN);
				// } else {
				// 	isHeatOn = 3;
				// 	bSwitch.text = "ON";
				// 	bSwitch.fill(RED);
				// }
				// delay(500);
				if (isHeatOn==0 || isHeatOn==2) {
					bSwitchU.text = "ON";
					bSwitchU.fill(RED);
					isHeatOn += 1;
				} else {
					bSwitchU.text = "OFF";
					bSwitchU.fill(GREEN);
					isHeatOn -= 1;
				}
			} else if (bSwitchD.isPressed(p)) {
				if (isHeatOn==0 || isHeatOn==1) {
					bSwitchD.text = "ON";
					bSwitchD.fill(RED);
					isHeatOn += 2;
				} else {
					bSwitchD.text = "OFF";
					bSwitchD.fill(GREEN);
					isHeatOn -= 2;
				}
			} else if (bBack.isPressed(p)) {
				bBack.fill(bPress);
				delay(200);
				// mainWindow();
				return;
			}
			// if ((x > 0) && (x < (tft.width()/2))) {
			// 	mainWindow();
			// }
		}  
	}
}



void graphProfile(int x, int y, int w, int h, Profile profile, bool detailed) {
	double maxTemp = profile.values[0][1];
	double maxTime = profile.values[profile.length-1][0];
	for (int i = 1; i<profile.length; i++) {
		if (profile.values[i][1]>maxTemp) {
			maxTemp = profile.values[i][1];
		}
	}

	for (int i = 0; i<profile.length-1; i++) {
		int x1 = map(long(profile.values[i][0]),0,long(maxTime),0,long(w));
		int y1 = map(long(profile.values[i][1]),0,long(maxTemp),0,long(h));

		int x2 = map(long(profile.values[i+1][0]),0,long(maxTime),0,long(w));
		int y2 = map(long(profile.values[i+1][1]),0,long(maxTemp),0,long(h));

		if (detailed) {
			tft.drawLine(x+x1,y+h-3,x+x1,y+h-y1,DARKGREY); // vertical lines
			tft.drawLine(x-3,y+h-y1,x+x1,y+h-y1,DARKGREY); // horizontal lines

			if (i==profile.length-2) {
				tft.drawLine(x-3,y+h-y2,x+x2,y+h-y2,DARKGREY);
				String temp1 = String(int(profile.values[i+1][1]));
				tft.setCursor(x-3-temp1.length()*5-3,y+h-y2-8/2+2);
				tft.println(temp1);
			}
		}

		tft.drawLine(x+x1,y+h-y1,x+x2-1,y+h-y2,cFRONT);

		tft.drawLine(x+x1,y+h-3,x+x1,y+h+3,cFRONT);
		tft.drawLine(x-3,y+h-y1,x+3,y+h-y1,cFRONT);

		tft.setTextColor(cFRONT);

		if (i==0 || detailed) {
			String time1 = String(int(profile.values[i][0]));
			tft.setCursor(x+x1-time1.length()*2.5,y+h+5);
			tft.setTextSize(1);
			tft.println(time1);

			String temp1 = String(int(profile.values[i][1]));
			tft.setCursor(x-3-temp1.length()*5-3,y+h-y1-8/2+2);
			tft.println(temp1);
		}
		if (i==profile.length-2) {
			tft.drawLine(x+x2-1,y+h-3,x+x2-1,y+h+3,cBACK);
			tft.drawLine(x-3,y+h-y2,x+3,y+h-y2,cFRONT);

			String time1 = String(int(profile.values[i+1][0]));
			tft.setCursor(x+x2-time1.length()*2.5,y+h+5);
			tft.setTextSize(1);
			tft.println(time1);
		}

		if (profile.values[i][1] == maxTemp) {
			String temp1 = String(int(profile.values[i][1]));
			tft.setCursor(x-3-temp1.length()*5-3,y+h-y1-8/2+2);
			tft.println(temp1);
		}

	}
}

void viewTemp(int x, int y) {
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



// void keyboard(int x, int y, int w, int h, int r, int c, char (*KB)[11]) {

// 	int space = 3;

// 	int kbWidth  = (w-space*(c-1))/c;
// 	int kbHeight = (h-space*r)/(r+1);

// 	tft.setTextSize(2);
// 	tft.setTextColor(cFRONT);

// 	for (int r0 = 0; r0<r; r0++) {
// 		int y1 = (kbHeight+space)*r0;
// 		for (int c0 = 0; c0<KB[r0][0]; c0++) {
// 			int x1 = (w-(kbWidth+space)*KB[r0][0]-space)/2 + (kbWidth+space)*c0;

// 			tft.drawRoundRect(x+x1,y+y1,kbWidth,kbHeight,7,cFRONT);

// 			tft.setCursor(x+x1+(kbWidth-10)/2,y+y1+space);
// 			tft.print(KB[r0][c0+1]);
			
// 			if (r0 == 3 && c0 == 6) {
// 				x1 += kbWidth+space;
// 				tft.drawRoundRect(x+x1,y+y1,w-(x+x1),kbHeight,7,cFRONT);
// 				tft.setCursor(x+x1+(w-(x+x1)-10)/2,y+y1+space*3);
// 				tft.print("<");

// 				y1 += (kbHeight+space);
// 				tft.drawRoundRect(x+x1,y+y1,w-(x+x1),kbHeight,7,cFRONT);
// 				tft.setCursor(x+x1+(w-(x+x1)-10)/2,y+y1+space*3);
// 				tft.print(">");
// 			}
// 		}
// 	}

	
// 	tft.drawRoundRect(x+(w-(kbWidth+space)*(KB[r-1][0])-space)/2,y+(kbHeight+space)*r,kbWidth*KB[r-1][0]+space*(KB[r-1][0]-1),kbHeight,7,cFRONT);
// }

// char keyboardB(int x, int y, int w, int h, int r0, int c0, char (*KB)[11], TSPoint p) {
// 	int space = 3;

// 	int kbWidth  = (w-space*(c0-1))/c0;
// 	int kbHeight = (h-space*r0)/(r0+1);

// 	p.x = map(p.x, TS_MINY, TS_MAXY, 0, tft.height());
// 	p.y = map(p.y, TS_MINX, TS_MAXX, 0, tft.width());
// 	int y0 = tft.height() - p.x;
// 	int x0 = p.y;

// 	for (int r = 0; r<r0; r++) {
// 		int y1 = (kbHeight+space)*r;
// 		for (int c = 0; c<KB[r][0]; c++) {
// 			int x1 = (w-(kbWidth+space)*KB[r][0]-space)/2 + (kbWidth+space)*c;

// 			if (x0>x+x1 && x0<x+x1+kbWidth && y0>y+y1 && y0<y+y1+kbHeight && p.z>ts.pressureThreshhold) {
// 				tft.fillRoundRect(x+x1,y+y1,kbWidth,kbHeight,7,bPress);
// 				delay(200);
// 				tft.fillRoundRect(x+x1,y+y1,kbWidth,kbHeight,7,cBACK);
// 				tft.drawRoundRect(x+x1,y+y1,kbWidth,kbHeight,7,cFRONT);

// 				tft.setCursor(x+x1+(kbWidth-10)/2,y+y1+space);
// 				tft.print(KB[r][c+1]);

// 				return KB[r][c+1];
// 			}
			
// 			if (r == 3 && c == 6 && p.z>ts.pressureThreshhold) {
// 				x1 += kbWidth+space;

// 				if (x0>x+x1 && x0<x+x1+w-(x+x1) && y0>y+y1 && y0<y+y1+kbHeight) {
// 					tft.fillRoundRect(x+x1,y+y1,w-(x+x1),kbHeight,7,bPress);
// 					delay(200);
// 					tft.fillRoundRect(x+x1,y+y1,w-(x+x1),kbHeight,7,cBACK);
// 					tft.drawRoundRect(x+x1,y+y1,w-(x+x1),kbHeight,7,cFRONT);
// 					tft.setCursor(x+x1+(w-(x+x1)-10)/2,y+y1+space*3);
// 					tft.print("<");
// 					return 1;
// 				}
				

// 				y1 += (kbHeight+space);

// 				if (x0>x+x1 && x0<x+x1+w-(x+x1) && y0>y+y1 && y0<y+y1+kbHeight) {
// 					tft.fillRoundRect(x+x1,y+y1,w-(x+x1),kbHeight,7,bPress);
// 					delay(200);
// 					tft.fillRoundRect(x+x1,y+y1,w-(x+x1),kbHeight,7,cBACK);
// 					tft.drawRoundRect(x+x1,y+y1,w-(x+x1),kbHeight,7,cFRONT);
// 					tft.setCursor(x+x1+(w-(x+x1)-10)/2,y+y1+space*3);
// 					tft.print(">");
// 					return 2;
// 				}
// 			}
// 		}
// 	}

// 	int bx = x+(w-(kbWidth+space)*(KB[r0-1][0])-space)/2;
// 	int by = y+(kbHeight+space)*r0;
// 	int bw = kbWidth*KB[r0-1][0]+space*(KB[r0-1][0]-1);
// 	int bh = kbHeight;

// 	if (x0>bx && x0<bx+bw && y0>by && y0<by+bh && p.z>ts.pressureThreshhold) {
// 		tft.fillRoundRect(bx,by,bw,bh,7,bPress);
// 		delay(200);
// 		tft.fillRoundRect(bx,by,bw,bh,7,cBACK);
// 		tft.drawRoundRect(bx,by,bw,bh,7,cFRONT);
// 		return ' ';
// 	}
// 	return 0;
// }