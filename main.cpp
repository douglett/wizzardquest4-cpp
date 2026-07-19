#include <iostream>
#include "src/globals.hpp"
#include "src/levelselect.hpp"
using namespace std;

// define globals
GFX gfx;
Texture2D textureTiles;
Texture2D textureSprites;
Texture2D textureExtras;
const Color PAL_BLACK = { 16, 8, 32, 255 };
// scene selection
LevelSelect levelselect;

int main() {
	printf("starting WizzardQuest 4!\n");
	gfx.init();
	gfx.screen.bgcolor = PAL_BLACK;

	textureTiles   = LoadTexture("assets/monotiles.png");
	textureSprites = LoadTexture("assets/sprites2.png");
	textureExtras  = LoadTexture("assets/extras.png");
	
	levelselect.allLevels();

	gfx.destroy();
}
