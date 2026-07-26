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

	virtual void init() { texture = textureSprites; }
	virtual void face(int mdir) { dir = mdir; }
	virtual void kill() { alive = false; printf("kill: %s, %lld\n", id.c_str(), (size_t)this); }

	virtual void paint(int xoff, int yoff) {
		if (!alive)  return;
		auto r = gfx.dir2point(dir, tsize);
		float rot = gfx.dir2rot(dir);
		gfx.blittr(textureExtras, tsize, 0, xoff+x+r.x, yoff+y+r.y, rot);
		Sprite::paint(xoff, yoff);
	}
};

// player
struct Wizzard : Mob {
	Wizzard() { id = "wizzard"; tile = 1; }
};

// enemies
struct Enemy : Mob {
	Enemy() { id = "enemy"; }
};

struct Slime : Enemy {
	Slime() { id = "slime"; tile = 0; }
};

struct Bear : Enemy {
	Bear() { id = "bear"; tile = 3; }
};

struct Archer : Enemy {
	Archer() { id = "archer"; tile = 4; }
};

struct WolfDog : Enemy {
	WolfDog() { id = "wolfdog"; tile = 5; }
};

// power up get
struct Spell : Mob {
	int spellid = 0, ispickup = false;
	Spell() { id = "spell"; texture = textureExtras; }
	virtual void face(int mdir) { dir = mdir; rot = gfx.dir2rot(dir); ispickup = false; }
	void setPickup(int id) { spellid = id; tile = 4 + id; face(1); ispickup = true; }
	void setPickup() { setPickup(spellid); }
	
	virtual void paint(int xoff, int yoff) {
		if (ispickup)  gfx.blitt(textureExtras, tsize, 4, xoff+x, yoff+y);
		Sprite::paint(xoff, yoff);
	}
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
