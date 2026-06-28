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
	// auto box1 = make_shared<Shape>();
	// 	box1->x = box1->y = 10;
	// 	box1->width = box1->height = 40;
	// 	box1->color = RED;
	
	// auto x = Shape{ 10, 10, 0, 40, 40, RED };
	// auto box1 = make_shared<ShapePaint>();
	// box1->p = x;
	// scene.append(box1);

	auto box1 = ShapePaint::make({ 10, 10, 0, 40, 40, RED });
	scene.append(box1);

	while (!gfx.shouldQuit()) {
		scene.paint(0, 0);
		gfx.flip();
	}
}
