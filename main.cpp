#include "globals.hpp"
#include "mobs.hpp"

GFX gfx;
Container scene;
Texture2D textureSprites;
Texture2D textureTiles;

int main() {
	printf("starting WizzardQuest 4!\n");
	gfx.init();
	mainloop();
	gfx.destroy();
}

void mainloop() {
	auto box1 = make_shared<Shape>();
		box1->x = box1->y = 10;
		box1->width = box1->height = 40;
		box1->color = RED;

	textureSprites = LoadTexture("../wizzardquest4/assets/sprites.png");
	textureTiles   = LoadTexture("../wizzardquest4/assets/monotiles.png");

	auto tmap = make_shared<TileMap>();
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

	while (!gfx.shouldQuit()) {
		if (wizzard->dir == -1) {
			if (IsKeyDown(KEY_UP))     wizzard->walk(0);
			if (IsKeyDown(KEY_RIGHT))  wizzard->walk(1);
			if (IsKeyDown(KEY_DOWN))   wizzard->walk(2);
			if (IsKeyDown(KEY_LEFT))   wizzard->walk(3);
		}

		scene.update();
		scene.paint(0, 0);
		gfx.flip();
	}
}

point dir2point(int dir, int mul) {
	switch (dir) {
		case 0:   return {  0, -1 };
		case 1:   return {  1,  0 };
		case 2:   return {  0,  1 };
		case 3:   return { -1,  0 };
		default:  return {  0,  0 };
	}
}
