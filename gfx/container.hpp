#pragma once
#include "raylib.h"
#include <vector>
#include <memory>
#include <algorithm>
using namespace std;

// paintable object base class
struct Paintable {
	using ptr = shared_ptr<Paintable>;
	string id;
	int x = 0, y = 0, z = 0;
	virtual void paint (int x, int y) {}
	virtual void update() {}
};

// paintable object with children
struct Container : Paintable {
	vector<Paintable::ptr> children;

	void append(Paintable::ptr p) { children.push_back(p); }
	void remove(Paintable::ptr p) { children.erase(find(children.begin(), children.end(), p)); }
	
	virtual void paint(int xoff, int yoff) {
		for (auto &c : children)
			c->paint(xoff + x, yoff + y);
	}

	virtual void update() {
		for (auto &c : children)
			c->update();
	}
};
