#include <iostream>
#include "Stage.h"

void Stage::setStagePlatform(std::vector<std::string> info) {
	float x = 0.0f, y = 0.0f;
	for (int i = 27; i >= 0; i--) {
		for (auto j = 0; j < info[i].size(); j++) {
			if ((j % 2) != 0) continue;

			if (info[i].substr(j, 2) == "бс") {
				Platform p(x, y, Platform::PLATFORM::GROUND);
				platforms.push_back(p);
			}
			else if (info[i].substr(j, 2) == "в├") {
				Platform p(x, y, Platform::PLATFORM::MIDDLE);
				platforms.push_back(p);
			}
			x += 1.0f;
		}
		x = 0.0f;
		y += 1.0f;
	}
}

void Stage::draw() {
	for (auto& p : platforms) {
		p.draw();
	}
}