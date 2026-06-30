#pragma once
#include "globals.hpp"

// struct Mob {};

struct Slime : Sprite {
	Slime() {
		id = "slime";
		texture = textureSprites;
		tile = 2;
	}

	virtual void update() {
		y++;
	}
};

struct Wizzard : Sprite {
	int dir = -1, step = 0;

	Wizzard() {
		id = "wizzard";
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
