#include "setup.h"

#pragma once

class Button {
	public:
		int x,y,w,h;
		String text;
		int txtSize = 2;
		Button();
		Button(int,int,int,int,String);
		void set(int,int,int,int,String);
		void view();
		bool isPressed(TSPoint p);
		void fill(uint16_t color);
};