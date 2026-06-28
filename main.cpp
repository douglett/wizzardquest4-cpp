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
	// shape box1 = {10, 10, 0, RED};
	// scene.append();

	while (!gfx.shouldQuit()) {
		scene.paint(0, 0);
		gfx.flip();
	}
}
