#pragma once
#include "container.hpp"

struct Shape : Paintable {
	int width = 10, height = 10;
	Color color;

	virtual void paint(int xpos, int ypos) {
		DrawRectangle(xpos+x, ypos+y, width, height, color);
	}
};
