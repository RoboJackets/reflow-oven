#pragma once

#include "setup.h"

class Button {
	public:
		int x,y,w,h;
		String text;
		int txtSize;
		Button(int,int,int,int,String);
		void set(int,int,int,int,String);
		void view();
		bool isPressed(TS_Point pnt);
		void fill(uint16_t color);
};
