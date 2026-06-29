#pragma once
#include "raylib.h"
#include "screen.hpp"
#include "container.hpp"
#include "shape.hpp"
#include "sprite.hpp"
#include "tilemap.hpp"

struct GFX {
	Screen screen;

	// forward to screen
	int  init()    { return screen.init(); }
	void destroy() { screen.destroy(); }
	void begin()   { screen.begin(); }
	void flip()    { screen.flip(); }

	// helpers
	bool shouldQuit() { return WindowShouldClose(); }
};
