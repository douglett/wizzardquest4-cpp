#pragma once
#include "container.hpp"

struct Shape : Paintable {
	int width = 10, height = 10;
	Color color;

	virtual void paint(int xoff, int yoff) {
		DrawRectangle(xoff+x, yoff+y, width, height, color);
	}
};
