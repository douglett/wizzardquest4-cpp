#pragma once
#include "globals.hpp"

struct Mob : Sprite {
	string state = "idle";
	int dir = 2, step = 0;
	bool alive = true;

	Mob() {
		id = "mob";
		texture = textureSprites;
		face(dir);
	}

	void face(int wdir) { dir = tile = wdir; }
	void walk(int wdir) { dir = wdir, step = 0, state = "walk"; }

	void kill() {
		printf("kill: %s, %lld\n", id.c_str(), (size_t)this);
		alive = false;
	}

	virtual void update() {
		if (state != "walk")  return;
		tile = dir;
		auto p = GFX::dir2point(dir);
		x += p.x, y += p.y;
		step++;
		if (step >= tsize)
			state = "idle", step = 0;
	}
};

struct Enemy : Mob {};

struct Slime : Enemy {
	Slime() {
		id = "slime";
	}
};

struct Wizzard : Mob {
	Wizzard() {
		id = "wizzard";
	}
};

struct Explosion : Container {
	int tsize = 16, step = 0;
	bool visible = false;

	Explosion() {
		id = "explosion";
		for (int i = 0; i < 4; i++) {
			auto ex = make_shared<ShapeCircle>();
				ex->radius = 3;
				ex->color = PINK;
				this->append(ex);
		}
	}

	void spawn(int tx, int ty) {
		x = tx * tsize;
		y = ty * tsize;
		step = 0;
		visible = true;
		for (auto c : children)
			c->x = c->y = tsize/2;
	}

	virtual void update() {
		if (step > tsize*3)  visible = false;
		if (!visible)   return;
		for (int i = 0; i < 4; i++) {
			auto r = gfx.dir2point(i);
			auto c = this->children.at(i);
			c->x += r.x;
			c->y += r.y;
		}
		step++;
	}

	virtual void paint(int xoff, int yoff) {
		if (!visible)  return;
		Container::paint(xoff, yoff);
	}
};
