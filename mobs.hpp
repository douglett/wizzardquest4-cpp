#pragma once
#include "globals.hpp"

// struct Mob {};

struct Slime : Sprite {
	Slime() {
		id = "slime";
		texture = textureSprites;
		tile = 2;
	}

	virtual void update() {
		y++;
	}
};
