#include "globals.hpp"
#include "cmath"

struct TileMapEx : TileMap {
	struct tmapmob { string type; int tx, ty, dir; };
	vector<tmapmob> mobs;

	virtual void reset() {
		TileMap::reset();
		mobs = {};
	}

	virtual void loadex(pugi::xml_document& doc) {
		// grab tileset start offset
		// auto tset = doc.select_node("map/tileset[@name='sprites2']");
		// int startTile = tset.node().attribute("firstgid").as_int();
		
		// grab objects
		auto result = doc.select_nodes("map/objectgroup[@name='mobs']/object");
		for (auto obj : result) {
			// object properties
			// int tile = obj.node().attribute("gid").as_int() - startTile;
			string type = obj.node().attribute("type").as_string();
			int x = obj.node().attribute("x").as_int();
			int y = obj.node().attribute("y").as_int();
			int dir = obj.node().select_node("properties/property[@name='dir']")
				.node().attribute("value").as_int();
			int tx = round(double(x) / tsize);
			int ty = round((double(y) - tsize) / tsize);
			// add object to list
			mobs.push_back({ type, tx, ty, dir });
		}
	}
};
