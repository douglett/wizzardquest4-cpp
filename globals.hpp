#pragma once
#include "gfx/gfx.hpp"
using namespace std;

extern GFX gfx;
extern Container scene;
extern Texture2D textureSprites;
extern Texture2D textureTiles;

struct point { int x, y; };
void  mainloop();
point dir2point(int dir, int mul=1);
