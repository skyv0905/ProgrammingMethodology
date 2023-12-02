#pragma once
#include <vector>
#include <string>
#include <memory>
#include "GL/freeglut.h"
#include "Constants.h"
#include "Platform.h"
#include "Texture.h"
#include "Enemy.h"

class Stage
{
public:
	Stage(int ID) : stageID(ID) {
		platform_texture_type1 = 1;
		platform_texture_type2 = 1;
		platform_texture_type3 = 1;
		firstTransition = 0;
		secondTransition = 0;
		player_origin.setPos(0.0f, 0.0f, 0.0f);
		enemy_count = 0;
	};

	void setStagePlatformTextureID(GLuint a, GLuint b, GLuint c); // 플랫폼에 넣을 텍스쳐의 ID 설정
	void setStagePlatform(std::vector<std::string> info);
	std::vector<Platform>& getStagePlatform();
	std::vector<std::shared_ptr<Enemy>>& getStageEnemy();
	bool stageEnds();

	float getFirstTransition(); // 화면 전환 관리
	float getSecondTransition();
	void startFirstTransition();
	void startSecondTransition();

	// DEAD된 ENEMY 제거
	void deleteDeadEnemy();

	Vector3f getPlayerOrigin();

	void draw();
	void move();

private:
	int stageID;
	int enemy_count; // 남아있는 적의 마릿수
	GLuint platform_texture_type1; // 플랫폼에 넣을 텍스쳐의 ID
	GLuint platform_texture_type2; // 최대 3개
	GLuint platform_texture_type3; // type1 : ■에 사용 / type2 : ▣에 사용 / type3 : 미정
	std::vector<Platform> platforms;
	std::vector<std::shared_ptr<Enemy>> enemys;

	float firstTransition; // 화면 전환 관리
	float secondTransition;

	Vector3f player_origin; // 플레이어 시작 위치
};

