#include "globals.hpp"
#include "mobs.hpp"

GFX gfx;
Container scene;
shared_ptr<TileMap> tmap;
Texture2D textureSprites;
Texture2D textureTiles;

void  mainloop();

int main() {
	printf("starting WizzardQuest 4!\n");
	gfx.init();
	mainloop();
	gfx.destroy();
}

int collide(Mob &mob, int dir) {
	// map collision
	auto r = gfx.dir2point(dir);
	auto c = tmap->at(mob.tx() + r.x, mob.ty() + r.y).collision;
	if (c)  return c;
	// mob collision
	for (auto c : scene.children) {
		if (c.get() == &mob)  continue;
		if (auto m = static_pointer_cast<Mob>(c)) {
			if (mob.tx()+r.x == m->tx() && mob.ty()+r.y == m->ty())
				return 1;
		}
	}
	return 0;
}

void mainloop() {
	auto box1 = make_shared<Shape>();
		box1->x = box1->y = 10;
		box1->width = box1->height = 40;
		box1->color = RED;

	textureSprites = LoadTexture("../wizzardquest4/assets/sprites.png");
	textureTiles   = LoadTexture("../wizzardquest4/assets/monotiles.png");

	tmap = make_shared<TileMap>();
		tmap->load("../wizzardquest4/assets/level1.tmx");
		tmap->texture = textureTiles;
		scene.append(tmap);

	auto wizzard = make_shared<Wizzard>();
		wizzard->tpos(1, 1);
		scene.append(wizzard);

	auto slime = make_shared<Slime>();
		slime->tpos(3, 1);
		scene.append(slime);

	scene.x = (gfx.screen.width  - tmap->twidth*tmap->tsize ) / 2;
	scene.y = (gfx.screen.height - tmap->theight*tmap->tsize) / 2;

	string state = "rest";

	while (!gfx.shouldQuit()) {
		if (state == "wwalk") {
			if (wizzard->dir == -1) state = "ewalk";
		}
		if (state == "ewalk") {
			// move enemies here
			state = "rest";
		}
		if (state == "rest") {
			if (IsKeyDown(KEY_UP)    && !collide(*wizzard, 0))  wizzard->walk(0), state = "wwalk";
			if (IsKeyDown(KEY_RIGHT) && !collide(*wizzard, 1))  wizzard->walk(1), state = "wwalk";
			if (IsKeyDown(KEY_DOWN)  && !collide(*wizzard, 2))  wizzard->walk(2), state = "wwalk";
			if (IsKeyDown(KEY_LEFT)  && !collide(*wizzard, 3))  wizzard->walk(3), state = "wwalk";
		}

		scene.update();
		scene.paint(0, 0);
		gfx.flip();
	}
}
