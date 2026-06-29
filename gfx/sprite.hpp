#pragma once
#include "container.hpp"

struct Sprite : Paintable {
	string id;
	int tsize = 16, tile = 0;
	Texture2D texture;

	void paint(int xoff, int yoff) {
		Screen::blitt(texture, tsize, tile, xoff+x, yoff+y);
	}
};
