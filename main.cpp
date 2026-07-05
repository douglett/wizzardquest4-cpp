#include "globals.hpp"
#include "mobs.hpp"

GFX gfx;
Container scene;
Texture2D textureSprites;
Texture2D textureTiles;
shared_ptr<TileMap> tmap;
shared_ptr<Wizzard> wizzard;
shared_ptr<Explosion> explosion;

void  mainloop();

int main() {
	printf("starting WizzardQuest 4!\n");
	gfx.init();
	mainloop();
	gfx.destroy();
}

int collideMap(Mob &mob, int dir) {
	// map collision
	auto r = gfx.dir2point(dir);
	return tmap->at(mob.tx() + r.x, mob.ty() + r.y).collision;
}

void killMob(int tx, int ty, int dir) {
	auto r = gfx.dir2point(dir);
	for (auto c : scene.children) {
		if (auto m = dynamic_pointer_cast<Mob>(c)) {
			if (m->tx() == tx+r.x && m->ty() == ty+r.y) {
				m->kill();
				scene.remove(m);
				explosion->spawn(m->tx(), m->ty());
			}
		}
	}
}

int killPlayer(Mob &mob, int dir) {
	auto r = gfx.dir2point(dir);
	if (mob.tx()+r.x == wizzard->tx() && mob.ty()+r.y == wizzard->ty()) {
		mob.walk(mob.dir);
		wizzard->kill();
		scene.remove(wizzard);
		explosion->spawn(wizzard->tx(), wizzard->ty());
		return true;
	}
	return false;
}

// void clearDead() {
// 	for (auto c : scene.children)
// 		if (auto m = dynamic_pointer_cast<Mob>(c))
// 			if (!m->alive) {
// 				printf("removing: %s\n", m->id.c_str());
// 				scene.remove(m);
// 			}
// }

void mainloop() {
	auto box1 = make_shared<ShapeRectangle>();
		box1->x = box1->y = 10;
		box1->width = box1->height = 40;
		box1->color = RED;

	textureSprites = LoadTexture("../wizzardquest4/assets/sprites.png");
	textureTiles   = LoadTexture("../wizzardquest4/assets/monotiles.png");

	tmap = make_shared<TileMap>();
		tmap->load("../wizzardquest4/assets/level1.tmx");
		tmap->texture = textureTiles;
		// tmap->debug = true;
		scene.append(tmap);

	wizzard = make_shared<Wizzard>();
		wizzard->tpos(1, 1);
		scene.append(wizzard);

	explosion = make_shared<Explosion>();
		// explosion->spawn(3, 3);
		scene.append(explosion);

	auto slime = make_shared<Slime>();
		slime->tpos(3, 1);
		slime->face(3);
		scene.append(slime);

	scene.x = (gfx.screen.width  - tmap->twidth*tmap->tsize ) / 2;
	scene.y = (gfx.screen.height - tmap->theight*tmap->tsize) / 2;

	string state = "rest";

	while (!gfx.shouldQuit()) {
		if (state == "wwalk") {
			// animate player moving
			if (wizzard->state == "idle") state = "ewalkstart";
		}
		if (state == "ewalkstart") {
			// move enemies
			for (auto c : scene.children)
				if (auto m = dynamic_pointer_cast<Slime>(c))
					killPlayer(*m, m->dir);
			state = "ewalk";
		}
		if (state == "ewalk") {
			// animate enemies moving
			int moving = 0;
			for (auto c : scene.children)
				if (auto m = dynamic_pointer_cast<Mob>(c))
					moving += (m->state != "idle");
			if (moving == 0)
				state = "rest";
		}
		if (state == "rest" && wizzard->alive) {
			int dir = -1;
			if      (IsKeyDown(KEY_UP))     dir = 0;
			else if (IsKeyDown(KEY_RIGHT))  dir = 1;
			else if (IsKeyDown(KEY_DOWN))   dir = 2;
			else if (IsKeyDown(KEY_LEFT))   dir = 3;
			// move animation
			if (dir > -1) {
				wizzard->face(dir);
				if (!collideMap(*wizzard, dir))
					wizzard->walk(dir),
					state = "wwalk",
					killMob(wizzard->tx(), wizzard->ty(), dir);
			}
		}

		scene.update();
		scene.paint(0, 0);
		gfx.flip();
	}
}
