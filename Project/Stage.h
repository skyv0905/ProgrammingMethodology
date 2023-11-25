#pragma once
#include <vector>
#include <string>
#include "GL/freeglut.h"
#include "Constants.h"
#include "Platform.h"
#include "Texture.h"

class Stage
{
public:
	Stage(int ID) : stageID(ID) {
		platform_texture_type1 = 1;
		platform_texture_type2 = 1;
		platform_texture_type3 = 1;
		firstTransition = 0;
		secondTransition = 0;
	};

	void setStagePlatformTextureID(GLuint a, GLuint b, GLuint c); // 플랫폼에 넣을 텍스쳐의 ID 설정
	void setStagePlatform(std::vector<std::string> info);
	std::vector<Platform>& getStagePlatform();

	float getFirstTransition(); // 화면 전환 관리
	float getSecondTransition();
	void startFirstTransition();
	void startSecondTransition();

	Vector3f getPlayerOrigin();

	Vector3f getEnemyOrigin1();
	Vector3f getEnemyOrigin2();
	Vector3f getEnemyOrigin3();

	void draw();
	void move();

private:
	int stageID;
	GLuint platform_texture_type1; // 플랫폼에 넣을 텍스쳐의 ID
	GLuint platform_texture_type2; // 최대 3개
	GLuint platform_texture_type3; // type1 : ■에 사용 / type2 : ▣에 사용 / type3 : 미정
	std::vector<Platform> platforms;

	float firstTransition; // 화면 전환 관리
	float secondTransition;

	Vector3f player_origin; // 플레이어 시작 위치
	Vector3f enemy_origin1;
	Vector3f enemy_origin2;
	Vector3f enemy_origin3;
};

