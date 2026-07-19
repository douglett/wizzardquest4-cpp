#pragma once
#include "paintable.hpp"
#include "pugixml.hpp"
#include <sstream>

struct TileMap : Paintable {
	struct tiledata { int tile, collision; };
	int twidth = 0, theight = 0, tsize = 16;
	vector<int> data, cdata;
	Texture2D texture;
	int debug = false, boundscollide = 0;

	int load(const string& fname) {
		reset();
		// open doc
		id = fname;
		pugi::xml_document doc;
		auto result = doc.load_file(fname.c_str());
		if (!result)
			return fprintf(stderr, "map xml load failed: %s\n", result.description()), 0;
		// load data
		twidth  = doc.child("map").attribute("width").as_int();
		theight = doc.child("map").attribute("height").as_int();
		// load layers data
		loadlayer(doc, data,  "map/layer[@name='map']/data");
		loadlayer(doc, cdata, "map/layer[@name='collision']/data");
		loadex   (doc);
		// sanity check
		if (twidth == 0 || theight == 0 || twidth*theight != (int)data.size() || data.size() != cdata.size())
			return printf("map parse error\n"), -1;
		// ok
		printf("map loaded '%s': w: %d, h: %d\n", fname.c_str(), twidth, theight);
		return 1;
	}

	virtual void reset() {
		twidth = theight = 0;
		data = cdata = {};
	}

	void loadlayer(pugi::xml_document& doc, vector<int>& mdata, const string& xpath) {
		// load 
		auto result = doc.select_node(xpath.c_str());
		string s = result.node().child_value();
		// cout << s << endl;
		// put data into vector
		stringstream ss(s);
		int i = 0;
		while (!ss.eof())
			if (ss >> i)
				mdata.push_back(i);
			else
				ss.clear(), ss.get();
		// cout << data.size() << endl;
	}

	// load extra data (override)
	virtual void loadex(pugi::xml_document& doc) {}

	void paint(int xoff, int yoff) {
		Color color = { 255, 0, 0, 64 };
		for (int yy = 0; yy < theight; yy++)
		for (int xx = 0; xx < twidth; xx++) {
			// show map tile
			int t = data.at(yy * twidth + xx);
			if (t > 0)
				Screen::blitt(texture, tsize, t-1, xoff+x + xx*tsize, yoff+y + yy*tsize);
			// show collision layer
			int c = cdata.at(yy * twidth + xx);
			if (c > 0 && debug)
				DrawRectangle(xoff+x + xx*tsize, yoff+y + yy*tsize, tsize, tsize, color);
		}
	}

	tiledata at(int tx, int ty) {
		if (tx < 0 || ty < 0 || tx >= twidth || ty >= theight)
			return { 0, boundscollide };
		return { data[ty*twidth+tx], cdata[ty*twidth+tx] };
	}

	void set(int tx, int ty, int tile, int collide) {
		if (tx < 0 || ty < 0 || tx >= twidth || ty >= theight)
			return;
		data [ty*twidth+tx] = tile;
		cdata[ty*twidth+tx] = collide;
	}
};
