#pragma once
#include <vector>
#include <memory>
#include <algorithm>
using namespace std;

// paintable object base class
struct Paintable {
	int x = 0, y = 0, z = 0;
	void paint(int x, int y) {}
};

// paintable object with children
struct Container : Paintable {
	// typedef shared_ptr<Container> ContainerP;
	using ContainerP = shared_ptr<Container>;
	vector<ContainerP> children;

	void append(ContainerP c) { children.push_back(c); }
	void remove(ContainerP c) { children.erase(find(children.begin(), children.end(), c)); }
	
	void paint(int x, int y) {
		for (auto &c : children)
			c->paint(x, y);
	}
};
