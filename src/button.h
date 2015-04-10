#pragma once

#include <stdint.h>
#include "TestPoint.h"

class Button {
	public:

		Button(){};
		Button(int, int, int, int, String&){
			x = x1; y = y1;
			w = w1; h = h1;
			text = text1;
		}

		void set(int, int, int, int, String&);
		void view(void);
		void fill(uint16_t color);
		bool isPressed(TSPoint&);

		int x, y, w, h, txtSize;
		String text;
};