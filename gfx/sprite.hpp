#pragma once
#include "container.hpp"

struct Sprite : Paintable {
	string id;
	int tsize = 16, tile = 0;
	Texture2D texture;

	void paint(int xoff, int yoff) {
		int tx = tile % (texture.width / tsize);
		int ty = tile / (texture.width / tsize);
		float t = tsize;
		Rectangle src{ tx*t, ty*t, t, t };
		Vector2   dst{ float(xoff+x), float(yoff+y) };
		DrawTextureRec(texture, src, dst, WHITE);
	}
};
