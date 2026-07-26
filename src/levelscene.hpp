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
	Container spells;
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
			if (m.type == "wizzard") {
				player.tpos(m.tx, m.ty);
				player.face(m.dir);
			} else if (m.type == "slime") {
				auto mob = make_shared<Slime>();
				mob->tpos(m.tx, m.ty);
				mob->face(m.dir);
				mobs.append(mob);
			} else if (m.type == "bear") {
				auto mob = make_shared<Bear>();
				mob->tpos(m.tx, m.ty);
				mob->face(m.dir);
				mobs.append(mob);
			} else if (m.type == "spell") {
				auto mob = make_shared<Spell>();
				mob->tpos(m.tx, m.ty);
				mob->setPickup(m.dir);  // use dir as spell id override for now
				mobs.append(mob);
			} else {
				printf("Error: unknown mob type: '%s'\n", m.type.c_str());
			}

		lvlname = to_string(levelno) + " - " +name;
		return 1;
	}

	int mainloop() {
		xpaint();
		while (!gfx.shouldQuit()) {
			if      (!player.alive)                          return 1;
			else if (IsKeyPressed(KEY_R))                    return 1;
			else if (playerOnExit())                         return 2;
			else if (player.alive && IsKeyDown(KEY_C))       pcast();
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
		if (collideMap(player, dir))
			return xpaint(); // paint at least one frame
		// kill any mobs in this direction
		auto r = gfx.dir2point(dir);
		for (auto c : mobs.children) {
			if (auto m = dynamic_pointer_cast<Enemy>(c)) {
				if (player.tx()+r.x == m->tx() && player.ty()+r.y == m->ty()) {
					m->kill();
					mobs.remove(m);
					explode(*m);
				}
			} else if (auto m = dynamic_pointer_cast<Spell>(c)) {
				if (player.tx()+r.x == m->tx() && player.ty()+r.y == m->ty()) {
					m->kill();
					mobs.remove(m);
					addSpell(m);
				}
			}
		}
		// walk player
		r = gfx.dir2point(dir);
		for (int i = 0; i < tsize; i++) {
			player.x += r.x, player.y += r.y;
			xpaint();
		}
		// walk mobs (one at a time)
		for (auto c : mobs.children) {
			// slime - static unless player is in front
			if (auto mob = dynamic_pointer_cast<Slime>(c)) {
				// on collision, walk mob and play explosion animation
				r = gfx.dir2point(mob->dir);
				if (mob->tx()+r.x == player.tx() && mob->ty()+r.y == player.ty()) {
					player.kill();
					explode(player);
					for (int i = 0; i < tsize; i++) {
						mob->x += r.x, mob->y += r.y;
						xpaint();
					}
				}
			}
			// bear - guard patrol
			else if (auto mob = dynamic_pointer_cast<Bear>(c)) {
				// on collision, walk mob and play explosion animation
				r = gfx.dir2point(mob->dir);
				if (mob->tx()+r.x == player.tx() && mob->ty()+r.y == player.ty()) {
					player.kill();
					explode(player);
				}
				if (!collideMap(*mob, mob->dir)) {
					for (int i = 0; i < tsize; i++) {
						mob->x += r.x, mob->y += r.y;
						xpaint();
					}
					if (collideMap(*mob, mob->dir))
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

	void pcast() {
		player.blend = { 255, 100, 100, 255 };
		int dir = -1;
		if      (IsKeyDown(KEY_UP))      dir = 0;
		else if (IsKeyDown(KEY_RIGHT))   dir = 1;
		else if (IsKeyDown(KEY_DOWN))    dir = 2;
		else if (IsKeyDown(KEY_LEFT))    dir = 3;
		if (dir >= 0) player.face(dir);
		xpaint();
		player.blend = WHITE;
		// no cast
		if (dir == -1 || spells.children.size() == 0 || collideMap(player, dir))
			return;
		// cast
		auto spell = dynamic_pointer_cast<Spell>(spells.children.back());
		spells.remove(spell);
		mobs.append(spell);
		auto pos = gfx.dir2point(dir);
		spell->x = player.x + pos.x*tsize;
		spell->y = player.y + pos.y*tsize;
		spell->face(dir);
		xpaint();
		// move spell
		while (true) {
			if (auto e = collideEnemy(*spell)) {
				spell->kill();
				mobs.remove(spell);
				e->kill();
				explode(*e);
				mobs.remove(e);
				break;
			}
			if (collideMap(*spell, dir)) {
				spell->setPickup();
				break;
			}
			for (int i = 0; i < tsize; i++) {
				spell->x += pos.x, spell->y += pos.y;
				xpaint();
			}
		}
		// update level
		openDoor();
	}

	int collideMap(Mob &mob, int dir=-1, int dist=1) {
		for (int d = 1; d <= dist; d++) {
			auto r = gfx.dir2point(dir, d);
			auto t = tmap.at(mob.tx() + r.x, mob.ty() + r.y);
			if (t.collision)  return 1;
			if (t.tile > 0 && t.tile != TILE_EXIT)  return t.tile;
		}
		return 0;
	}
	
	shared_ptr<Enemy> collideEnemy(Mob& mob, int dir=-1) {
		auto pos = gfx.dir2point(dir);
		for (auto c : mobs.children)
			if (auto m = dynamic_pointer_cast<Enemy>(c))
				if (mob.tx()+pos.x == m->tx() && mob.ty()+pos.y == m->ty())
					return m;
		return NULL;
	}

	int playerOnExit() {
		return tmap.at(player.tx(), player.ty()).tile == TILE_EXIT;
	}

	void explode(Mob& mob) {
		explosions.append( make_shared<Explosion>(mob.tx(), mob.ty()) );
	}

	void addSpell(shared_ptr<Spell> p) {
		spells.append(p);
		p->x = p->y = 0;
		spells.x = gfx.screen.width - (tsize * spells.children.size());
		spells.y = gfx.screen.height - tsize;
	}

	void removeSpell() {
		spells.children.pop_back();
		spells.x = gfx.screen.width - (tsize * spells.children.size());
		spells.y = gfx.screen.height - tsize;
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
		spells.paint(0, 0);
		gfx.text(lvlname, 1, gfx.screen.height - 10, GREEN);
	}
	// update, paint, flip
	void xpaint() {
		update();
		paint();
		gfx.flip();
	}
};
