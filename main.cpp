#include "gfx/gfx.hpp"
GFX gfx;

int main() {
	printf("hello world 1\n");
	printf("hello world 2\n"); 
	printf("hello world 3\n");

	gfx.init();
	while (!gfx.shouldQuit()) {
		// here
		gfx.flip();
	}
	gfx.destroy();
}
