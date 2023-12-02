#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <chrono>

#include <queue>
#include <map>
#include <cmath>

#include "Constants.h"
#include "Stage.h"
#include "Platform.h"
#include "Texture.h"
#include "CollisionHandler.h"

#include "Light.h"
#include "Player.h"
#include "Enemy.h"
#include "Life.h"

#include "OpenALSoundHandler.h"
#include "GL/freeglut.h"

using namespace std;

clock_t start_t = clock();
clock_t end_t;
clock_t fps;

string NAME = "BubbleBobble!";
string string1 = "Press Space Bar to Start";

StageState state; // 스테이지 상태
LoadState load; // 로드 상태

vector<Stage> stages;
vector<Bubble> bubbles;
vector<Texture> textures;

OpenALSoundHandler alh;

// 키보드 입력 상태

bool bPressLeft;
bool bPressRight;
bool bPressUp;
bool bPressDown;

bool fpsShow;
bool PlayerIsOnPlatform;

bool debugmode = true; // 디버깅모드

int globalTimeCount = 0; // 0~60

Player player(0, 0, 0.0f, PLAYER_SIZE);
Life life(PIXEL, Vector3f(-boundaryX + PIXEL / 2, -boundaryY + PIXEL / 2, 0));

Light light(boundaryX, boundaryY, boundaryX / 2, GL_LIGHT0);

CollisionHandler colHandler;

void initialize() {
	// bool init
	bPressLeft = false;
	bPressRight = false;
	bPressUp = false;
	bPressDown = false;
	fpsShow = false;
	PlayerIsOnPlatform = false;

	// 메인화면 이미지 로딩 0
	Texture mainImage;
	mainImage.initializeTexture("resources/Bubble_Bobble_Cover.jpeg");
	textures.push_back(mainImage);

	// 플랫폼 이미지 로딩 1 ~ 6
	auto num_img = 6; // 로딩 개수
	string prefix_platform = "resources/Platform_type";
	for (auto i = 1; i <= num_img; i++) {
		Texture image;
		cout << (prefix_platform + to_string(i) + ".png").c_str();
		image.initializeTexture((prefix_platform + to_string(i) + ".png").c_str());
		textures.push_back(image);
	}

	// 플레이어 이미지 로딩 7
	Texture playerimage;
	playerimage.initializeTexture("resources/Player.png");
	textures.push_back(playerimage);

	// Enemy 이미지 로딩 8
	Texture enemyimage;
	enemyimage.initializeTexture("resources/Enemy.png");
	textures.push_back(enemyimage);

	// GameOver 이미지 로딩 9
	Texture gameover;
	gameover.initializeTexture("resources/Game_Over.png");
	textures.push_back(gameover);

	// Life 이미지 로딩 10 ~ 13
	auto num_life_img = 4; // 로딩 개수
	string prefix_life = "resources/Life";
	for (auto i = 1; i <= num_life_img; i++) {
		Texture image;
		cout << (prefix_life + to_string(i) + ".png").c_str();
		image.initializeTexture((prefix_life + to_string(i) + ".png").c_str());
		textures.push_back(image);
	}

	// MAIN
	Stage main(0);
	stages.push_back(main);

	// STAGE 1
	// Platform 생성
	Stage stage1(1);
	stage1.setStagePlatformTextureID(textures[1].getTextureID(), textures[2].getTextureID(), textures[3].getTextureID());
	vector<string> platformInfo;
	platformInfo.push_back("■■■■■■■■■■■            ■■■■■■■■■■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("                                                        ");
	platformInfo.push_back("                                                        ");
	platformInfo.push_back("                                                        ");
	platformInfo.push_back("                          ◀                            ");
	platformInfo.push_back("■■▣▣      ▣▣▣▣▣▣▣▣▣▣▣▣▣▣      ▣▣■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                      ▶                        ■■");
	platformInfo.push_back("■■▣▣      ▣▣▣▣▣▣▣▣▣▣▣▣▣▣      ▣▣■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                      ◀                        ■■");
	platformInfo.push_back("■■▣▣      ▣▣▣▣▣▣▣▣▣▣▣▣▣▣      ▣▣■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■      ▲                                        ■■");
	platformInfo.push_back("■■■■■■■■■■■            ■■■■■■■■■■■");

	stage1.setStagePlatform(platformInfo);
	stages.push_back(stage1);
	platformInfo.clear();

	// STAGE 2
	// Platform 생성
	Stage stage2(2);
	stage2.setStagePlatformTextureID(textures[2].getTextureID(), textures[5].getTextureID(), textures[3].getTextureID());
	platformInfo.push_back("■■■■■■■■■■■            ■■■■■■■■■■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■▣▣▣▣▣▣      ▣▣▣▣▣▣      ▣▣▣▣▣▣■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("                                                        ");
	platformInfo.push_back("                                                        ");
	platformInfo.push_back("                                                        ");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                  ▶                            ■■");
	platformInfo.push_back("■■            ■■■■■■■■■■■■            ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                ◀                ◀            ■■");
	platformInfo.push_back("■■▣▣      ▣▣▣▣▣        ▣▣▣▣▣      ▣▣■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                      ▶                        ■■");
	platformInfo.push_back("■■▣▣▣▣▣▣      ▣▣▣▣▣▣      ▣▣▣▣▣▣■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                          ▲    ■■");
	platformInfo.push_back("■■■■■■■■■■■            ■■■■■■■■■■■");

	stage2.setStagePlatform(platformInfo);
	stages.push_back(stage2);
	platformInfo.clear();

	// STAGE 3
	// Platform 생성
	Stage stage3(3);
	stage3.setStagePlatformTextureID(textures[3].getTextureID(), textures[4].getTextureID(), textures[1].getTextureID());
	platformInfo.push_back("■■                ■■■■■■■■                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("                                                        ");
	platformInfo.push_back("                                                        ");
	platformInfo.push_back("    ▶                                                  ");
	platformInfo.push_back("■■▣▣▣▣▣▣                                    ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                ▶                              ■■");
	platformInfo.push_back("■■                ■■■■■■■■                ■■");
	platformInfo.push_back("■■                              ■                ■■");
	platformInfo.push_back("■■                              ■                ■■");
	platformInfo.push_back("■■                              ■                ■■");
	platformInfo.push_back("■■                              ■                ■■");
	platformInfo.push_back("■■                              ■                ■■");
	platformInfo.push_back("■■                          ◀  ■                ■■");
	platformInfo.push_back("■■                ■■■■■■■■                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("                                                        ");
	platformInfo.push_back("                                                        ");
	platformInfo.push_back("    ▶                                                  ");
	platformInfo.push_back("■■▣▣▣▣▣▣                                    ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                        ▲                      ■■");
	platformInfo.push_back("■■                ■■■■■■■■                ■■");

	stage3.setStagePlatform(platformInfo);
	stages.push_back(stage3);
	platformInfo.clear();

	// 게임 STATE init
	state = StageState::BEGIN;
	load = LoadState::NONE;

	//Player의 초기 State setting
	player.setVerticalState(Player::VERTICAL_STATE::STOPV);
	player.setHorizontalState(Player::HORIZONTAL_STATE::STOPH);
	player.setExState(Player::FORCEMOVING);

	// light 초기 설정
	light.setAmbient(0.5f, 0.5f, 0.5f, 1.0f);
	light.setDiffuse(0.7f, 0.7f, 0.7f, 1.0f);
	light.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	
	//GAME OVER state에서의 stage
	Stage end(0);
	stages.push_back(end);
}

// Stage와 Player간의 collision handling

bool handleCollisionX(Player& player, Platform& platform) {

	auto w = platform.getWidth();
	auto d = (PLAYER_SIZE + w) / 2; // 접할 때 거리
	auto center_pl = player.getCenter();
	auto center_pf = platform.getCenter();
	auto center_plb = player.getCenterB();
	float dx = center_pl[0] - center_pf[0];
	float dy = center_pl[1] - center_pf[1];

	Vector4f ltrb_pl(center_pl[0] - PLAYER_SIZE / 2, center_pl[1] + PLAYER_SIZE / 2, center_pl[0] + PLAYER_SIZE / 2, center_pl[1] - PLAYER_SIZE / 2); // 플레이어 좌표 LEFT / TOP / RIGHT / BOTTOM
	Vector4f ltrb_pf(center_pf[0] - w / 2, center_pf[1] + w / 2, center_pf[0] + w / 2, center_pf[1] - w / 2); // 플랫폼 좌표

	if (abs(dx) < d && abs(dy) < d) { // 충돌 발생
		Vector3f v = center_pl - center_plb;
		Vector3f new_center(center_pl);

		auto side = -1;

		if (v[0] < 0) { // (0 : 왼) , (1 : 위) , (2 : 오른) , (3 : 아래)
			side = 0;
		}
		else if (v[0] > 0) {
			side = 2;
		}

		if (platform.getPlatformType() == Platform::PLATFORM::MIDDLE) return 1; // 플랫폼 타입이 MIDDLE인 경우 좌우 충돌은 핸들링하지 않음, 1을 return하여 Y collision함수에 해당 상황 전달

		switch (side) {
		case 0:
			if (debugmode) cout << "플레이어 충돌 발생 (왼쪽)" << endl;
			new_center[0] = ltrb_pf[2] + PLAYER_SIZE / 2; // 플랫폼 오른쪽 좌표 + (플레이어 사이즈 / 2)
			player.setCenter(new_center);
			player.setHorizontalState(Player::STOPH);
			break;
		case 2:
			if (debugmode) cout << "플레이어 충돌 발생 (오른쪽)" << endl;
			new_center[0] = ltrb_pf[0] - PLAYER_SIZE / 2; // 플랫폼 왼쪽 좌표 - (플레이어 사이즈 / 2)
			player.setCenter(new_center);
			player.setHorizontalState(Player::STOPH);
			break;
		}
	}
	return 0;
}

void handleCollisionY(Player& player, Platform& platform, int collisionDetectedX) {

	auto w = platform.getWidth();
	auto d = (PLAYER_SIZE + w) / 2; // 접할 때 거리
	auto center_pl = player.getCenter();
	auto center_pf = platform.getCenter();
	auto center_plb = player.getCenterB();
	float dx = center_pl[0] - center_pf[0];
	float dy = center_pl[1] - center_pf[1];

	Vector4f ltrb_pl(center_pl[0] - PLAYER_SIZE / 2, center_pl[1] + PLAYER_SIZE / 2, center_pl[0] + PLAYER_SIZE / 2, center_pl[1] - PLAYER_SIZE / 2); // 플레이어 좌표 LEFT / TOP / RIGHT / BOTTOM
	Vector4f ltrb_pf(center_pf[0] - w / 2, center_pf[1] + w / 2, center_pf[0] + w / 2, center_pf[1] - w / 2); // 플랫폼 좌표

	if (abs(dx) < d && abs(dy) < d) { // 충돌 발생
		Vector3f v = center_pl - center_plb;
		Vector3f new_center(center_pl);

		auto side = -1;

		if (v[1] < 0) { // (0 : 왼) , (1 : 위) , (2 : 오른) , (3 : 아래)
			side = 3;
		}
		else if (v[1] > 0) {
			side = 1;
		}

		switch (side) {
		case 1:
			if (platform.getPlatformType() == Platform::PLATFORM::MIDDLE) break;
			if (debugmode) cout << "플레이어 충돌 발생 (위쪽)" << endl;
			new_center[1] = ltrb_pf[3] - PLAYER_SIZE / 2; // 플랫폼 아래쪽 좌표 - (플레이어 사이즈 / 2)
			player.setCenter(new_center);
			break;
		case 3:
			if (player.getVelocity()[1] > 0 || collisionDetectedX) break;
			if (debugmode) cout << "플레이어 충돌 발생 (아래쪽)" << endl;
			new_center[1] = ltrb_pf[1] + PLAYER_SIZE / 2; // 플랫폼 위쪽 좌표 + (플레이어 사이즈 / 2)
			player.setCenter(new_center);
			PlayerIsOnPlatform = true;
			break;
		}
	}

	return;
}

// 플레이어 아래의 바닥 유무 감지
bool bottomDetector(Player& player, Platform& platform, int i) {
	auto w = platform.getWidth();
	auto d = (PLAYER_SIZE + w) / 2; // 접할 때 거리
	auto center_pl = player.getCenter();
	auto center_plb = player.getCenterB();
	auto center_pf = platform.getCenter();
	float dx = center_pl[0] - center_pf[0];
	float dy = center_pl[1] - center_pf[1];
	float dy2 = center_plb[1] - center_pf[1];

	//if (abs(dx) < d && abs(dy) <= d && player.getVelocity()[1] <= 0) {
	if (abs(dx) < d && (center_pl[1] - center_pf[1] - d) <= 0 && (center_plb[1] - center_pf[1] - d) >= 0) {
		//cout << "바닥" << endl;
		return true;
	}
	else {
		return false;
	}
}

// 연결된 버블 감지
bool bubbleChainCollisionDetector(Vector3f c1, Vector3f c2, float r1, float r2) {
	auto distance = sqrt(pow((c1[0] - c2[0]), 2) +
		pow((c1[1] - c2[1]), 2) +
		pow((c1[2] - c2[2]), 2));
	if (distance <= r1 + r2) {
		return true;
	}
	else return false;
}

// 버블 충돌 시 연결된 모든 버블 제거
void bubbleCollisionHandler(int i) {
	queue<int> q;
	q.push(i);
	bubbles[i].setDeleted();
	while (!q.empty()) {
		int t = q.front();
		for (auto j = 0; j < bubbles.size(); j++) {
			if (bubbles[j].isWillDeleted()) continue;

			auto c1 = bubbles[j].getCenter(); // 체크할 대상
			auto c2 = bubbles[t].getCenter(); // 기준
			auto r1 = bubbles[j].getRadius();
			auto r2 = bubbles[t].getRadius();
			bool willdelete = false;

			if (bubbleChainCollisionDetector(c1, c2, r1, r2)) {
				willdelete = true;
			}

			if (c2[1] + r2 > boundaryY && bubbleChainCollisionDetector(c1, c2 + Vector3f(0.0f, -WINDOW_HEIGHT, 0.0f), r1, r2)) { // 경계를 넘어선 버블에 대한
				willdelete = true;
			}
			else if (c2[1] - r2 < -boundaryY && bubbleChainCollisionDetector(c1, c2 + Vector3f(0.0f, WINDOW_HEIGHT, 0.0f), r1, r2)) {
				willdelete = true;
			}

			if (c2[0] + r2 > boundaryX && bubbleChainCollisionDetector(c1, c2 + Vector3f(-WINDOW_WIDTH, 0.0f, 0.0f), r1, r2)) {
				willdelete = true;
			}
			else if (c2[0] - r2 < -boundaryX && bubbleChainCollisionDetector(c1, c2 + Vector3f(WINDOW_WIDTH, 0.0f, 0.0f), r1, r2)) {
				willdelete = true;
			}

			if (willdelete) {
				bubbles[j].setDeleted(); // 삭제할 버블에 willDeleted 체크
				q.push(j);
			}
		}
		q.pop();
	}
}

// DELETED 체크된 버블 모두 Vector에서 제거
void deleteWillDeletedBubbles() {
	for (int i = bubbles.size(); i > 0; i--) {
		if (bubbles[i - 1].isWillDeleted()) {
			bubbles[i - 1].setTrappedEnemyDead();
			alh.playMusicBubblePopped();
			bubbles.erase(bubbles.begin() + i - 1);
			if (debugmode) cout << i - 1 << "번째 버블 삭제" << endl;
		}
	}
}

// 모든 버블 제거
void deleteAllBubbles() {
	bubbles.clear();
}

void idle() {

	end_t = clock();

	if ((float)(end_t - start_t) > 1000 / 30.0f) { // 프레임 제어

		fps = end_t - start_t;
		if (fps > 0) fps = 1000 / fps;

		start_t = end_t;
		globalTimeCount = (globalTimeCount + 1) % 60;

		if (state == StageState::BEGIN) return; // IDLE함수는 state가 begin이면 아래를 실행하지 않음.
		if (state == StageState::OVER) return; // IDLE함수는 state가 OVER이면 아래를 실행하지 않음.

		alh.playMusicBackground();
		/* ▼ 아래는 로드 중일때는 실행되지 않음 ▼ */

		if (load == NONE) {

			if (alh.sourceStateBackground == AL_STOPPED) { alh.playMusicBackground(); }

			if (!player.isInvincible()) {
				colHandler(player, stages[static_cast<int>(state)].getStageEnemy());
			}

			if (player.getUnderAttack()) {
				player.setExState(Player::EX_STATE::INVINCIBLE);
				player.setUnderAttack(false);

				player.loseLife(); // life 감소

				if (player.getLife() == 0) { // life  == 0

					state = StageState::OVER;
					alh.playMusicGameOver();
 
					alSourceStop(alh.sourcebackground);
					alSourceStop(alh.sourcepopped);
					alSourceStop(alh.sourceshotted);
					return;
				}
			}

		// 플레이어와 버블 움직임을 업데이트 하는 부분
			bool isBubbleCollisionDetected = false;
			for (auto i = 0; i < bubbles.size(); i++) { // 버블 이동
				bubbles[i].move();

				// 적 - 버블간 충돌
				if (bubbles[i].getState() == Bubble::GROWING) {
					for (auto& e : stages[static_cast<int>(state)].getStageEnemy()) {
						if (e->isTrapped() || e->isDead()) continue;

						auto d = bubbles[i].getRadius() + (e->getSize() / 2); // 접할 때 거리
						auto center_b = bubbles[i].getCenter();
						auto center_e = e->getCenter();
						float dx = center_e[0] - center_b[0];
						float dy = center_e[1] - center_b[1];

						if (abs(dx) < d && abs(dy) < d) { // 충돌 발생
							if (debugmode) cout << "적과 버블 충돌 발생\n";
							auto mid = (center_b + center_e) / 2;
							mid[2] = 0.0f;
							e->setExState(Enemy::TRAPPED);
							e->setCenter(mid);
							bubbles[i].handleCollisionWEnemy(mid, e);
						}
					}
				}

				else {
					// 플레이어 - 버블간 충돌

					auto d = bubbles[i].getRadius() + (PLAYER_SIZE / 2); // 접할 때 거리
					auto center_b = bubbles[i].getCenter();
					auto center_p = player.getCenter();
					float dx = center_p[0] - center_b[0];
					float dy = center_p[1] - center_b[1];

					if (abs(dx) < d && abs(dy) < d) { // 충돌 발생
						bubbleCollisionHandler(i); // 버블 충돌 시 연결된 모든 버블 제거
						isBubbleCollisionDetected = true; // 버블과 충돌 확인
					}
				}
			}
			if (isBubbleCollisionDetected) { // 버블과 충돌 확인 시
				deleteWillDeletedBubbles(); // DELETED 체크된 버블 모두 Vector에서 제거
			}
			stages[static_cast<int>(state)].deleteDeadEnemy(); // DEAD된 ENEMY 제거

			// 충돌 제어
			int i = 0; // 디버깅 용도
			bool findBottom = false;
			int collisionDetectedX = 0; // MIDDLE 플랫폼에서 충돌이 발생했는지 체크
			if (player.isMoving()) {
				player.moveX(); // 플레이어 X이동
			}
			for (auto& platform : stages[static_cast<int>(state)].getStagePlatform()) { // X충돌감지
				collisionDetectedX += handleCollisionX(player, platform);
			}
			if (player.isJumping() || player.isFalling()) { // Y충돌감지
				player.moveY(); // 플레이어Y 이동
				for (auto& platform : stages[static_cast<int>(state)].getStagePlatform()) {
					handleCollisionY(player, platform, collisionDetectedX);
				}
			}
			for (auto& platform : stages[static_cast<int>(state)].getStagePlatform()) {
				// 플랫폼 - 버블간 충돌

				if (platform.getPlatformType() != Platform::PLATFORM::MIDDLE) {
					for (auto& bubble : bubbles) {
						if (bubble.getState() == Bubble::STOP) {
							continue;
						}

						auto d = bubble.getRadius() + (platform.getWidth() / 2); // 접할 때 거리
						auto center_b = bubble.getCenter();
						auto center_p = platform.getCenter();
						float dx = center_p[0] - center_b[0];
						float dy = center_p[1] - center_b[1];

						if (abs(dx) < d && abs(dy) < d) { // 충돌 발생
							if (debugmode) cout << i << "번째 플랫폼에 버블 충돌 발생" << endl;
							bubble.handleCollision(center_p, platform.getWidth() / 2);
						}
					}
				}
				if (!findBottom && (!player.isJumping() || (player.isJumping() && player.getVelocity()[1] < 0)) && bottomDetector(player, platform, i)) { // 바닥이 있는지 감지
					findBottom = true;
				}
				i += 1; // 디버깅용도
			}
			if (findBottom) { // 바닥이 있음 감지 시 STOP 설정
				PlayerIsOnPlatform = true;
				player.setVerticalState(Player::STOPV);
			}
			else if (!findBottom && !player.isJumping() && !player.isFalling()) { // 바닥이 없음 감지 시 FALL 설정
				PlayerIsOnPlatform = false;
				player.setVerticalState(Player::FALL);
			}

			if (bPressLeft || bPressRight) { // 방향 전환

				player.setFace(bPressLeft ? Player::LEFT : Player::RIGHT);
				player.setHorizontalState(player.HORIZONTAL_STATE::MOVE);
			}
			else player.setHorizontalState(Player::STOPH);

			for (auto& e : stages[static_cast<int>(state)].getStageEnemy()) {
				e->move();
				e->setFace(e->getFace());
			}

			// 스테이지 클리어 체크
			if (stages[static_cast<int>(state)].stageEnds()) {
				auto nextStage = static_cast<int>(state) + 1;
				if (debugmode) cout << "스테이지 " << nextStage << "로 이동" << endl;
				deleteAllBubbles(); // 모든 버블 제거
				state = static_cast<StageState>(nextStage); // 스테이지 + 1
				stages[nextStage - 1].startSecondTransition(); // 이전 화면 전환 효과
				stages[nextStage].startFirstTransition(); // 다음 화면 전환 효과
				load = LOAD_STAGE; // 로드 상태 설정
			}
		}

		/* ▲ 위는 로드 중일때는 실행되지 않음 ▲ */

		// 스테이지 전환
		if (load == LOAD_STAGE) {
			stages[static_cast<int>(state) - 1].move();
			stages[static_cast<int>(state)].move();

			if (stages[static_cast<int>(state)].getFirstTransition() == 0) { // 로드 상태
				load = LoadState::LOAD_PLAYER;
				player.moveTo(stages[static_cast<int>(state)].getPlayerOrigin(), 20.0f);
			}
		}

		if (load == LOAD_PLAYER) { // 플레이어 로드

			player.move();
			player.mMoveTick();

			if (player.moveFinished()) {
				player.setHorizontalState(Player::STOPH);
				player.setVerticalState(Player::STOPV);

				load = LoadState::LOAD_ENEMY;

				for (auto& e : stages[static_cast<int>(state)].getStageEnemy()) {
					e->setVerticalState(Enemy::VERTICAL_STATE::FALL);
					e->moveTo(20.0f);
				}
			}
		}

		if (load == LOAD_ENEMY) { // 적 로드

			for (auto& e : stages[static_cast<int>(state)].getStageEnemy()) {
				e->move();
				e->mMoveTick();

				if (e->moveFinished()) {
					load = LoadState::NONE;

					e->setHorizontalState(Enemy::HORIZONTAL_STATE::MOVE);
					e->setVerticalState(Enemy::VERTICAL_STATE::STOPV);
				}
			}
		}

		/* ▼ 아래는 로드 상태와 상관없이 실행됨 ▼ */
		if (!player.canShootBubble()) { // 버블 재발사 대기시간 제어
			player.mBubbleCooldown();
		}
		if (player.isInvincible()) {
			player.mInvincible();
		}
		/* ▲ 위는 로드 상태와 상관없이 실행됨 ▲ */

		// 프레임 제어 끝
	}

	glutPostRedisplay();
}

void display() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (state == StageState::BEGIN) {

		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glBindTexture(GL_TEXTURE_2D, textures[0].getTextureID());
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-350, -350);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-350, 350);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(350, 350);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(350, -350);
		glEnd();
		glDisable(GL_TEXTURE_2D);

		glPushMatrix();
		glRasterPos2f(-115, -250);
		for (int i = 0; i < string1.size(); i++)
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string1[i]);
		glPopMatrix();
	}
	else if (state == StageState::OVER) {

		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glBindTexture(GL_TEXTURE_2D, textures[9].getTextureID());
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-350, -350);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-350, 350);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(350, 350);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(350, -350);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
	else if (state != StageState::BEGIN) {
		glPushMatrix(); // 화면 전환 효과
		if (stages[static_cast<int>(state)].getFirstTransition()) glTranslatef(0, stages[static_cast<int>(state)].getFirstTransition(), 0); // 새로운 화면
		stages[static_cast<int>(state)].draw();
		glPopMatrix();

		if (stages[static_cast<int>(state) - 1].getSecondTransition()) {
			glPushMatrix();
			glTranslatef(0, stages[static_cast<int>(state) - 1].getSecondTransition() + WINDOW_HEIGHT, 0);  // 이전 화면
			stages[static_cast<int>(state) - 1].draw();
			glPopMatrix();
		} // 화면 전환 효과 끝

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//3D 요소들 draw
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glEnable(light.getID());

		light.draw();

		//버블 draw
		glPushMatrix();
		glTranslatef(0, 0, -100.0f);
		for (auto& bubble : bubbles) {
			bubble.draw();
		}
		glPopMatrix();
		glDisable(light.getID());
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);

		for (auto& e : stages[static_cast<int>(state)].getStageEnemy()) {
			e->draw();
		}

		for (auto i = 0; i < player.getLife(); i++) {
			life.draw(i);
		}

		player.draw();

		glDisable(GL_BLEND);
	}

	if (fpsShow) {
		glPushMatrix(); // 프레임 업데이트 시간 확인
		glRasterPos2f(-boundaryX + PLAYER_SIZE, boundaryY - PLAYER_SIZE);
		string string_fps = "FPS: " + to_string(fps) + "ms";
		glColor3f(1.0f, 1.0f, 1.0f);
		for (int i = 0; i < string_fps.size(); i++)
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string_fps[i]);
		glPopMatrix();
	}

	glutSwapBuffers();
}

void keyboardDown(unsigned char key, int x, int y) {

	// Space 누르면 Player가 bubble 생성하여 bubbles vector에 넣기

	if (key == 32) {

		if (state == StageState::BEGIN) {
			state = StageState::STAGE1;
			stages[static_cast<int>(state)].startFirstTransition();
			load = LOAD_STAGE;
		}

		else if (state != StageState::BEGIN) {
			if (player.canShootBubble() && load == NONE) {
				bubbles.push_back(player.shootBubble());
				alh.playMusicBubbleShotted();
			}
		}
	}

	/* 이 아래는 디버깅을 위한 코드입니다 */
	if (debugmode) {
		switch (key)
		{
		case 'V':
		case 'v':
		{
			Vector3f v = player.getVelocity();
			cout << "현재 플레이어의 속도: " << v[0] << ", " << v[1] << ", " << v[2] << endl;
			break;
		}
		case 'C':
		case 'c':
		{
			Vector3f c = player.getCenter();
			cout << "현재 플레이어의 중심: " << c[0] << ", " << c[1] << ", " << c[2] << endl;
			break;
		}
		case 'P':
		case 'p':
			player.printState();
			break;
		case 'F': // fps 표시
		case 'f':
			fpsShow = !fpsShow;
			break;
		case 'N': // 강제로 다음 스테이지로 전환
		case 'n':
			if (state != StageState::BEGIN) {
				auto nextStage = static_cast<int>(state) + 1;
				if (nextStage >= stages.size()) {
					cout << "마지막 스테이지" << endl;
					break;
				}
				else {
					cout << "스테이지 " << nextStage << "로 이동" << endl;
					deleteAllBubbles(); // 모든 버블 제거
					state = static_cast<StageState>(nextStage); // 스테이지 + 1
					stages[nextStage - 1].startSecondTransition(); // 이전 화면 전환 효과
					stages[nextStage].startFirstTransition(); // 다음 화면 전환 효과
					load = LOAD_STAGE; // 로드 상태 설정
				}
			}
			break;
		}
	}
}

void specialKeyDown(int key, int x, int y) {

	// 방향키 눌릴 때 Player가 움직이는 상태로 지정하여 속도만큼 위치를 업데이트할 수 있도록 (업데이트 여부는 Player의 move 함수에 구문 있음)
	if (key == GLUT_KEY_LEFT) {

		if (load == NONE) {
			bPressLeft = true;
		}
	}

	if (key == GLUT_KEY_RIGHT) {

		if (load == NONE) {
			bPressRight = true;
		}
	}

	if (key == GLUT_KEY_UP) {

		bPressUp = true;
		if (!player.isJumping() && !player.isFalling() && load == NONE) {
			player.setVerticalState(player.VERTICAL_STATE::JUMP);
			alh.playMusicJump();
		}
	}

	if (key == GLUT_KEY_DOWN) {

		bPressDown = true;
	}
}

void specialKeyUp(int key, int x, int y) {

	// 방향키 떨어질 때 Player가 멈추는 상태로 지정하여 위치를 업데이트 하지 않도록 (업데이트 여부는 Player의 move 함수에 구문 있음)

	switch (key) {

	case GLUT_KEY_LEFT:

		bPressLeft = false;
		break;

	case GLUT_KEY_RIGHT:

		bPressRight = false;
		break;

	case GLUT_KEY_UP:

		bPressUp = false;
		break;

	case GLUT_KEY_DOWN:

		bPressDown = false;
		break;
	}

}

void reshape(int w, int h) {

	glViewport(0, 0, w, h);

	double wRatio = (double)w / WINDOW_WIDTH;
	double hRatio = (double)h / WINDOW_HEIGHT;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-boundaryX * wRatio, boundaryX * wRatio, -boundaryY * hRatio, boundaryY * hRatio, -500.0, 500.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void gameLoop() {

	// init GLUT and create Window
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(WINDOW_X, WINDOW_Y);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Bubble Bobble");

	initialize();

	// register callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboardDown);
	glutSpecialFunc(specialKeyDown);
	glutSpecialUpFunc(specialKeyUp);

	// enter GLUT event processing cycle
	glutMainLoop();
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);

	gameLoop();
	alh.cleanUpAudio();

	return 0;
}