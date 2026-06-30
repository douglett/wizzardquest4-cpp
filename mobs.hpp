#pragma once
#include "globals.hpp"

struct Mob : Sprite {
	int dir = -1, step = 0;

	Mob() {
		id = "mob";
		texture = textureSprites;
		tile = 2;
	}

	void walk(int wdir) { dir = wdir, step = 0; }

	virtual void update() {
		if (dir < 0)  return;
		auto p = dir2point(dir);
		x += p.x, y += p.y;
		step++;
		if (step >= tsize)
			dir = -1, step = 0;
	}
};

struct Slime : Mob {
	Slime() {
		id = "slime";
	}
};

struct Wizzard : Mob {
	Wizzard() {
		id = "wizzard";
	}
};
