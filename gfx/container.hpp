#pragma once
#include "raylib.h"
#include <vector>
#include <memory>
#include <algorithm>
using namespace std;

// paintable object base class
struct Paintable {
	// using ptr = shared_ptr<Paintable>;
	int x = 0, y = 0, z = 0;
	void paint(int x, int y) {}
};

struct Paint {
	Paintable p;
	virtual void paint(int x, int y) {}
};

// paintable object with children
struct Container : Paintable {
	vector<shared_ptr<Paint>> children;

	void append(shared_ptr<Paint> p) { children.push_back(p); }
	void remove(shared_ptr<Paint> p) { children.erase(find(children.begin(), children.end(), p)); }
	
	void paint(int x, int y) {
		for (auto &c : children)
			c->paint(x, y);
	}
};
