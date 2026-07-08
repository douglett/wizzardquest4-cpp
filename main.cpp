#include <iostream>
#include "src/globals.hpp"
#include "src/levelscene.hpp"
using namespace std;

// define globals
GFX gfx;
Texture2D textureSprites;
Texture2D textureTiles;
// scene selection
LevelScene level;

int main() {
	printf("starting WizzardQuest 4!\n");
	gfx.init();

	textureSprites = LoadTexture("../wizzardquest4/assets/sprites.png");
	textureTiles   = LoadTexture("../wizzardquest4/assets/monotiles.png");
	level.init();
	level.mainloop();

	gfx.destroy();
}
