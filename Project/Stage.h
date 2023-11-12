#pragma once
#include <vector>
#include <string>
#include "GL/freeglut.h"
#include "Constants.h"
#include "Platform.h"

class Stage
{
public:
	Stage(int ID) : stageID(ID) {};

	void setStagePlatform(std::vector<std::string> info);
	void draw();

private:
	int stageID;
	std::vector<Platform> platforms;
};

