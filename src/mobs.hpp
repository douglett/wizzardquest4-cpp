#pragma once
#include "globals.hpp"
#include <cmath>

// base mob
struct Mob : Sprite {
	int alive = true, dir = 2;

	Mob() {
		id = "mob";
		texture = textureSprites;
		face(dir);
	}

	void init() { texture = textureSprites; }
	void face(int wdir) { dir = tile = wdir; }

	void kill() {
		printf("kill: %s, %lld\n", id.c_str(), (size_t)this);
		alive = false;
	}

	virtual void paint(int xoff, int yoff) {
		if (alive)  Sprite::paint(xoff, yoff);
	}
};

// player
struct Wizzard : Mob {
	Wizzard() { id = "wizzard"; }
};

// enemies
struct Enemy : Mob {
	Enemy() { id = "enemy"; }
};

struct Slime : Enemy {
	Slime() { id = "slime"; }
};

// misc
struct Explosion : Paintable {
	int alive = true, tsize = 16, frame = 0;

	Explosion(int tx, int ty) {
		id = "explosion";
		x = tx*tsize, y = ty*tsize;
	}

	virtual void update() {
		if (!alive)  return;
		frame++;
		if (frame > tsize*3)  alive = false;
	}

	virtual void paint(int xoff, int yoff) {
		if (!alive)  return;
		auto col = PINK; // base color
		col.a = min((1 - (double(frame) / (tsize*3))) * 2 * 255, 255.0); // fade out at the end
		int c = tsize/2; // center
		for (int i = 0; i < 8; i++) {
			int xx = cos(numbers::pi * 2 / 8 * i) * frame;
			int yy = sin(numbers::pi * 2 / 8 * i) * frame;
			DrawCircle(xoff+x+c+xx, yoff+y+c+yy, 3, col);
		}
	}
};
