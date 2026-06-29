#pragma once
#include "container.hpp"
#include "pugixml.hpp"

struct TileMap : Paintable {
	int width = 0, height = 0;

	int load(const string& fname) {
		// open doc
		pugi::xml_document doc;
		auto result = doc.load_file(fname.c_str());
		if (!result)
			return fprintf(stderr, "xml load failed: %s\n", result.description()), -1;
		// load data
		width = doc.child("map").attribute("width").as_int();
		height = doc.child("map").attribute("width").as_int();
		printf("%d %d\n", width, height);
		return 0;
	}
};
