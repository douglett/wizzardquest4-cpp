#include <iostream>
#include "src/globals.hpp"
#include "src/levelscene.hpp"
using namespace std;

// define globals
GFX gfx;
Texture2D textureTiles;
Texture2D textureSprites;
Texture2D textureExtras;
// scene selection
LevelScene level;

int main() {
	printf("starting WizzardQuest 4!\n");
	gfx.init();

	textureTiles   = LoadTexture("assets/monotiles.png");
	textureSprites = LoadTexture("assets/sprites2.png");
	textureExtras  = LoadTexture("assets/extras.png");
	level.init();
	level.mainloop();

	gfx.destroy();
}
