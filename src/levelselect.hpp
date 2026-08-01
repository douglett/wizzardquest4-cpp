#pragma once
#include "globals.hpp"
#include "levelscene.hpp"

struct LevelSelect {
	struct LevelType { string fname, name; };
	const vector<LevelType> levelinfo = {
		{},
		{ "sm1.tmx", "Walk Right" },
		{ "sm2.tmx", "Walk Right?" },
		{ "sm3.tmx", "Don't Be Hasty" },
		{ "sm4.tmx", "Bear in the Woods" },
		{ "sm5.tmx", "Porridge" },
		{ "sm6.tmx", "Magic Time" },
		{ "sm7.tmx", "Zoomin'" },
		{ "sm8.tmx", "Spikey'" },
	};
	int levelno = 8;
	LevelScene level;

	void allLevels() {
		while (levelno >= 1 && levelno < (int)levelinfo.size()) {
			auto& lvl = levelinfo.at(levelno);
			if (!level.load(levelno, lvl.fname, lvl.name))  return;  // abort on error
			int result = level.mainloop();
			switch (result) {
				case 0:  return;  // quit
				case 1:  break;   // death - retry
				case 2:  levelno++;  break;  // next level on win
			}
		}
	}
};
