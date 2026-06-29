#pragma once
#include "container.hpp"
#include "pugixml.hpp"
#include <sstream>

struct TileMap : Paintable {
	int width = 0, height = 0;
	vector<int> data, cdata;
	Texture2D texture;

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
		// load layers data
		loadlayer(doc, data,  "map/layer[@name='map']/data");
		loadlayer(doc, cdata, "map/layer[@name='collision']/data");
		// ok
		return 0;
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

	void paint(int xpos, int ypos) {
		for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++) {
			// TODO
		}
	}
};
