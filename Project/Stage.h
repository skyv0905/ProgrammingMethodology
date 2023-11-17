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
	std::vector<Platform> getStagePlatform();
	void draw();
	void move();

private:
	int stageID;
	std::vector<Platform> platforms;
};

