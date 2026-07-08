#pragma once
#include "globals.hpp"
#include "mobs.hpp"

struct LevelScene {
	const int TILE_DOOR = 16, TILE_EXIT = 12;
	int x = 0, y = 0, tsize = 16;
	TileMap tmap;
	Wizzard player;
	Container mobs;
	Container explosions;

	void init() {
		tmap.load("../wizzardquest4/assets/level1.tmx");
		tmap.texture = textureTiles;
		// tmap.debug = true;
		x = (gfx.screen.width  - tmap.twidth*tsize ) / 2;
		y = (gfx.screen.height - tmap.theight*tsize) / 2;

		for (int i = 0; i < 3; i++) {
			auto slime = make_shared<Slime>();
			slime->tpos(3, 1+i*2);
			// slime->face(3);
			mobs.append(slime);
		}

		player.init();
		player.tpos(1, 1);
	}

	int mainloop() {
		while (!gfx.shouldQuit()) {
			if      (!player.alive)                          return 0;
			else if (playerOnExit())                         return 1;
			else if (player.alive && IsKeyDown(KEY_UP))      pwalk(0);
			else if (player.alive && IsKeyDown(KEY_RIGHT))   pwalk(1);
			else if (player.alive && IsKeyDown(KEY_DOWN))    pwalk(2);
			else if (player.alive && IsKeyDown(KEY_LEFT))    pwalk(3);
			else    xpaint();
		}
		return 0;
	}

	void pwalk(int dir) {
		player.face(dir);
		if (collideMap(player, dir, 2))
			return xpaint(); // paint at least one frame
		// kill any mobs in this direction
		auto r = gfx.dir2point(dir, 2);
		for (auto c : mobs.children) {
			if (auto m = dynamic_pointer_cast<Enemy>(c)) {
				if (player.tx()+r.x == m->tx() && player.ty()+r.y == m->ty()) {
					m->kill();
					mobs.remove(m);
					explode(*m);
				}
			}
		}
		// walk player
		r = gfx.dir2point(dir, 2);
		for (int i = 0; i < tsize; i++) {
			player.x += r.x, player.y += r.y;
			xpaint();
		}
		// walk mobs (one at a time)
		for (auto c : mobs.children) {
			if (auto mob = dynamic_pointer_cast<Enemy>(c)) {
				r = gfx.dir2point(mob->dir, 2);
				if (mob->tx()+r.x == player.tx() && mob->ty()+r.y == player.ty()) {
					// on collision, walk mob and play explosion animation
					player.kill();
					explode(player);
					for (int i = 0; i < tsize; i++) {
						mob->x += r.x, mob->y += r.y;
						xpaint();
					}
				}
			}
		}
		// update level
		openDoor();
		// wait a bit if we are at end of level
		if (!player.alive || playerOnExit()) {
			printf("waiting...\n");
			for (int i = 0; i < 60; i++)
				xpaint();
			printf("end.\n");
		}
	}

	int collideMap(Mob &mob, int dir, int dist) {
		for (int d = 1; d <= dist; d++) {
			auto r = gfx.dir2point(dir, d);
			auto c = tmap.at(mob.tx() + r.x, mob.ty() + r.y).collision;
			if (c) return c;
		}
		return 0;
	}

	int playerOnExit() {
		return tmap.at(player.tx(), player.ty()).tile == TILE_EXIT;
	}

	void explode(Mob& mob) {
		explosions.append( make_shared<Explosion>(mob.tx(), mob.ty()) );
	}

	void openDoor() {
		if (mobs.children.size() > 0)  return;
		for (int y = 0; y < tmap.theight; y++)
		for (int x = 0; x < tmap.twidth; x++)
			if (tmap.at(x, y).tile == TILE_DOOR)
				tmap.set(x, y, 0, 0);
	}

	// background actions update
	void update() {
		explosions.update();
		for (auto c : explosions.children)
			if (auto ex = dynamic_pointer_cast<Explosion>(c))
				if (!ex->alive)
					explosions.remove(ex);
	}
	// repaint all
	void paint() {
		tmap.paint(x, y);
		mobs.paint(x, y);
		player.paint(x, y);
		explosions.paint(x, y);
	}
	// paint, update, flip
	void xpaint() {
		update();
		paint();
		gfx.flip();
	}
};


// struct Timer {
// 	int time = 0, tt = 0;
// 	bool started = false, finished = false;
// 	void start()   { tt = time, started = true, finished = false; }
// 	void update()  { if (started &&--tt == 0) finished = true; }
// 	int  running() { return started && !finished; }
// };
