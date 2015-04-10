#include "button.h"

void Button::set(int x1, int y1, int w1, int h1, const String& text1) {
	x = x1;
	y = y1;
	w = w1;
	h = h1;
	text = text1;
}

void Button::view(void) {
	tft.drawRoundRect(x,y,w,h,7,cFRONT);
	tft.setTextColor(cFRONT);
	tft.setTextSize(tSize);
	tft.setCursor(x+(w-txtSize*(text.length()+1)*5)*.5,y+(h-txtSize*8)*.5);
	tft.println(text);
}

void Button::fill(uint16_t color) {
	tft.fillRoundRect(x,y,w,h,7,color);
	view();
}

bool Button::isPressed(const TSPoint& p) {
	int y1 = tft.height() - map(p.x, TS_MINY, TS_MAXY, 0, tft.height());
	int x1 = map(p.y, TS_MINX, TS_MAXX, 0, tft.width());
	if ( x1>x & x1<x+w & y1>y & y1<y+h ) {
		return true;
	}
	return false;
}