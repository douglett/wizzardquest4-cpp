#pragma once
#include "container.hpp"

struct Sprite : Paintable {
	int tsize = 16, tile = 0;
	Texture2D texture;

	virtual void paint(int xoff, int yoff) {
		Screen::blitt(texture, tsize, tile, xoff+x, yoff+y);
	}

	virtual void tpos(int tx, int ty) { x = tx*tsize, y = ty*tsize; }
};
