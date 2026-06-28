#pragma once
#include "container.hpp"

struct Shape : Paintable {
	int width = 10, height = 10;
	Color color;

	void paint(int xpos, int ypos) {
		DrawRectangle(xpos+x, ypos+y, width, height, color);
	}
};

struct ShapePaint : Paint {
	Shape p;
	virtual void paint(int x, int y) { p.paint(x, y); }
	static shared_ptr<ShapePaint> make(const Shape& p) { auto s = make_shared<ShapePaint>(); s->p = p; return s; }
};
