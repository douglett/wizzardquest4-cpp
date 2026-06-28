#include "gfx/gfx.hpp"

GFX gfx;
Container scene;

void mainloop();

int main() {
	printf("starting WizzardQuest 4!\n");
	gfx.init();
	mainloop();
	gfx.destroy();
}

void mainloop() {
	// auto box1 = make_shared<Shape>(Shape{ 10, 10, 0, 40, 40, RED });
	auto box1 = make_shared<Shape>();
		box1->x = box1->y = 10;
		box1->width = box1->height = 40;
		box1->color = RED;
	scene.append(box1);

	auto sprite1 = make_shared<Sprite>();
		sprite1->x = sprite1->y = 60;
		sprite1->tile = 2;
		sprite1->texture = LoadTexture("../wizzardquest4/assets/sprites.png");
	scene.append(sprite1);

	while (!gfx.shouldQuit()) {
		scene.paint(0, 0);
		gfx.flip();
	}
}
