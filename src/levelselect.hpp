#pragma once
#include "globals.hpp"
#include "levelscene.hpp"

struct LevelSelect {
	struct LevelType { string fname, name; };
	const vector<LevelType> levelinfo = {
		{},
		{ "level1.tmx", "Walk Right" },
		{ "level2.tmx", "Walk Right?" },
	};
	int levelno = 1;
	LevelScene level;

	void allLevels() {
		while (levelno >= 1 && levelno < (int)levelinfo.size()) {
			auto& lvl = levelinfo.at(levelno);
			if (level.load(levelno, lvl.fname, lvl.name))  return;  // abort on error
			int result = level.mainloop();
			switch (result) {
				case -1:  return;  // quit
				case  0:  break;   // death - retry
				case  1:  levelno++;  break;  // next level on win
			}
		}
	}
};
