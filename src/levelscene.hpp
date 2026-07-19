#pragma once
#include "globals.hpp"
#include "mobs.hpp"
#include "tilemapex.hpp"

struct LevelScene {
	const int TILE_DOOR = 17, TILE_EXIT = 13, TILE_PATHH = 18, TILE_PATHV = 19;
	int x = 0, y = 0, tsize = 16;
	TileMapEx tmap;
	Wizzard player;
	Container mobs;
	Container explosions;
	string lvlname;

	int load(int levelno, const string& fname, const string& name) {
		// reset scene
		mobs = explosions = {};
		player.init();
		player.alive = true;
		
		// load
		if (!tmap.load("assets/"+fname))  return 0;
		tmap.texture = textureTiles;
		tmap.boundscollide = 1;
		// tmap.debug = true;
		x = (gfx.screen.width  - tmap.twidth*tsize ) / 2;
		y = (gfx.screen.height - tmap.theight*tsize) / 2;

		// init mobs
		for (auto m : tmap.mobs)
			if (m.type == "slime") {
				auto mob = make_shared<Slime>();
				mob->tpos(m.tx, m.ty);
				mob->face(m.dir);
				mobs.append(mob);
			} else if (m.type == "wizzard") {
				player.tpos(m.tx, m.ty);
				player.face(m.dir);
			} else {
				printf("Error: unknown mob type: '%s'\n", m.type.c_str());
			}

		lvlname = to_string(levelno) + " - " +name;
		return 1;
	}

	int mainloop() {
		while (!gfx.shouldQuit()) {
			if      (!player.alive)                          return 1;
			else if (playerOnExit())                         return 2;
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
			// slime - static unless player is in front
			if (auto mob = dynamic_pointer_cast<Slime>(c)) {
				// on collision, walk mob and play explosion animation
				r = gfx.dir2point(mob->dir, 2);
				if (mob->tx()+r.x == player.tx() && mob->ty()+r.y == player.ty()) {
					player.kill();
					explode(player);
					for (int i = 0; i < tsize; i++) {
						mob->x += r.x, mob->y += r.y;
						xpaint();
					}
				}
			}
			// guard - patrol
			else if (auto mob = dynamic_pointer_cast<Guard>(c)) {
				// on collision, walk mob and play explosion animation
				r = gfx.dir2point(mob->dir, 2);
				if (mob->tx()+r.x == player.tx() && mob->ty()+r.y == player.ty()) {
					player.kill();
					explode(player);
				}
				if (!collideMap(*mob, mob->dir, 2)) {
					for (int i = 0; i < tsize; i++) {
						mob->x += r.x, mob->y += r.y;
						xpaint();
					}
					if (collideMap(*mob, mob->dir, 2))
						mob->face((mob->dir + 2) % 4);
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
			if (tmap.at(x, y).tile == TILE_DOOR) {
				if (tmap.at(x, y-1).tile == TILE_EXIT || tmap.at(x, y+1).tile == TILE_EXIT)
					tmap.set(x, y, TILE_PATHV, 0);
				else if (tmap.at(x-1, y).tile == TILE_EXIT || tmap.at(x+1, y).tile == TILE_EXIT)
					tmap.set(x, y, TILE_PATHH, 0);
				else
					tmap.set(x, y, 0, 0);	
			}
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
		gfx.text(lvlname, 1, gfx.screen.height - 10, GREEN);
	}
	// update, paint, flip
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
