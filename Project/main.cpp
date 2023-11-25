#include <iostream>
#include "GL/freeglut.h"

#include "Constants.h"
#include "Stage.h"
#include "Platform.h"
#include "Texture.h"

#include "Light.h"
#include "Player.h"


using namespace std;

clock_t start_t = clock();
clock_t end_t;

string NAME = "BubbleBobble!";
string string1 = "Press Space Bar to Start";

enum StageState { BEGIN, STAGE1 };
enum LoadState { NONE, LOAD_STAGE, LOAD_PLAYER, LOAD_ENEMY };

StageState state;
LoadState load;

vector<Stage> stages;
vector<Bubble> bubbles;
vector<Texture> textures;

bool bPressLeft;
bool bPressRight;
bool bPressUp;
bool bPressDown;

bool PlayerIsOnPlatform;

Player player(0, 0, 0.0f, PLAYER_SIZE);
Light light(boundaryX, boundaryY, boundaryX / 2, GL_LIGHT0);

void initialize() {

	// 메인화면 이미지 로딩
	Texture mainImage;
	mainImage.initializeTexture("Bubble_Bobble_Cover.jpeg");
	textures.push_back(mainImage);

	// 플랫폼 이미지 로딩
	auto num_img = 6; // 로딩 개수
	string prefix_platform = "Platform_type";
	for (auto i = 1; i <= num_img; i++) {
		Texture image;
		cout << (prefix_platform + to_string(i) + ".png").c_str();
		image.initializeTexture((prefix_platform + to_string(i) + ".png").c_str());
		textures.push_back(image);
	}

	// 플레이어 이미지 로딩
	Texture playerimage;
	playerimage.initializeTexture("Player.png");
	textures.push_back(playerimage);

	// MAIN
	Stage main(0);
	stages.push_back(main);

	// STAGE 1
	// Platform 생성
	Stage stage1(1);
	stage1.setStagePlatformTextureID(textures[1].getTextureID(), textures[2].getTextureID(), textures[3].getTextureID());
	vector<string> platformInfo;
	platformInfo.push_back("■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■              ■■                              ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■▣▣      ▣▣▣▣▣▣▣▣▣▣▣▣▣▣      ▣▣■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■▣▣      ▣▣▣▣▣▣▣▣▣▣▣▣▣▣      ▣▣■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■▣▣      ▣▣▣▣▣▣▣▣▣▣▣▣▣▣      ▣▣■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■      ▲                                        ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■■■■■■■■■■■■■■■■■■■■■■■■■■■");


	stage1.setStagePlatform(platformInfo);
	stages.push_back(stage1);
	platformInfo.clear();

	state = BEGIN;
	load = NONE;

	//Player의 초기 State setting
	player.setVerticalState(Player::VERTICAL_STATE::STOPV);
	player.setHorizontalState(Player::HORIZONTAL_STATE::STOPH);
	player.setExState(Player::FORCEMOVING);

	// light 초기 설정
	light.setAmbient(0.5f, 0.5f, 0.5f, 1.0f);
	light.setDiffuse(0.7f, 0.7f, 0.7f, 1.0f);
	light.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
}

// Stage와 Player간의 collision handling

template <typename T>
T abs(T num) {
	return num >= 0 ? num : -num;
}

void handleCollisionX(Player& player, Platform& platform, int i) { // 마지막 int i 는 디버깅용도임

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
		Vector3f v;
		for (auto i = 0; i < 3; i++) {
			v[i] = center_pl[i] - center_plb[i];
		}
		Vector3f new_center(center_pl);

		char side;

		if (v[0] < 0) { // 반대로 쫒아갈 때 먼저 부딪히는 면 / (0 : 왼) , (1 : 위) , (2 : 오른) , (3 : 아래)
			side = 0;
		}
		else if (v[0] > 0) {
			side = 2;
		}
		else return;

		switch (side) {
		case 0:
			if (platform.getPlatformType() == Platform::PLATFORM::MIDDLE) break;
			cout << i << "번째 플랫폼에 플레이어 충돌 발생 ";
			new_center[0] = ltrb_pf[2] + PLAYER_SIZE / 2; // 플랫폼 오른쪽 좌표 + (플레이어 사이즈 / 2)
			cout << "(왼쪽)" << new_center[0] << endl;
			player.setCenter(new_center);
			player.setHorizontalState(Player::STOPH);
			break;
		case 2:
			if (platform.getPlatformType() == Platform::PLATFORM::MIDDLE) break;
			cout << i << "번째 플랫폼에 플레이어 충돌 발생 ";
			cout << "(오른쪽)" << endl;
			new_center[0] = ltrb_pf[0] - PLAYER_SIZE / 2; // 플랫폼 왼쪽 좌표 - (플레이어 사이즈 / 2)
			player.setCenter(new_center);
			player.setHorizontalState(Player::STOPH);
			break;
		}
	}

	return;
}

void handleCollisionY(Player& player, Platform& platform, int i) { // 마지막 int i 는 디버깅용도임

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
		Vector3f v;
		for (auto i = 0; i < 3; i++) {
			v[i] = center_pl[i] - center_plb[i];
		}
		Vector3f new_center(center_pl);

		char side;

		if (v[1] < 0) { // 반대로 쫒아갈 때 먼저 부딪히는 면 / (0 : 왼) , (1 : 위) , (2 : 오른) , (3 : 아래)
			side = 3;
		}
		else if (v[1] > 0) {
			side = 1;
		}
		else return;

		switch (side) {
		case 1:
			if (platform.getPlatformType() == Platform::PLATFORM::MIDDLE) break;
			cout << i << "번째 플랫폼에 플레이어 충돌 발생 ";
			cout << "(위쪽)" << endl;
			new_center[1] = ltrb_pf[3] - PLAYER_SIZE / 2; // 플랫폼 아래쪽 좌표 - (플레이어 사이즈 / 2)
			player.setCenter(new_center);
			break;
		case 3:
			if (player.getVelocity()[1] > 0) break;
			cout << i << "번째 플랫폼에 플레이어 충돌 발생 ";
			cout << "(아래쪽)" << endl;
			new_center[1] = ltrb_pf[1] + PLAYER_SIZE / 2; // 플랫폼 위쪽 좌표 + (플레이어 사이즈 / 2)
			player.setCenter(new_center);
			PlayerIsOnPlatform = true;
			break;
		}
	}

	return;
}

bool bottomDetector(Player& player, Platform& platform, int i) {
	auto w = platform.getWidth();
	auto d = (PLAYER_SIZE + w) / 2; // 접할 때 거리
	auto center_pl = player.getCenter();
	auto center_plb = player.getCenterB();
	auto center_pf = platform.getCenter();
	float dx = center_pl[0] - center_pf[0];
	float dy = center_pl[1] - center_pf[1];
	float dy2 = center_plb[1] - center_pf[1];

	if (abs(dx) < d && abs(dy) <= d && player.getVelocity()[1] <= 0) {
		//cout << "바닥" << endl;
		return true;
	}
	else {
		return false;
	}
}

void idle() {

	end_t = clock();

	if ((float)(end_t - start_t) > 1000 / 30.0f) { // 프레임 제어

		/* ▼ 아래는 로드 중일때는 실행되지 않음 ▼ */

		if (load == NONE) {
		// 플레이어와 버블 움직임을 업데이트 하는 부분

			//if (player.isMoving()) {
			//	player.move(); // 플레이어 이동
			//}

			/*for (int i = 0; i < stages[1].getStagePlatform().size(); ++i) {
				handleCollision(player, stages[1].getStagePlatform()[i]);
			}*/

			for (auto& bubble : bubbles) { // 버블 이동
				bubble.move();
			}

			// 충돌 제어
			int i = 0;
			bool findBottom = false;
			if (player.isMoving() || player.isJumping() || player.isFalling()) {
				if (player.isMoving()) {
					player.moveX();
					for (auto& platform : stages[state].getStagePlatform()) {
						handleCollisionX(player, platform, i);
					}
				}
				if (player.isJumping() || player.isFalling()) {
					player.moveY();
					for (auto& platform : stages[state].getStagePlatform()) {
						handleCollisionY(player, platform, i);
					}
				}
			}
			for (auto& platform : stages[state].getStagePlatform()) {
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
						dx *= dx < 0 ? -1 : 1;
						dy *= dy < 0 ? -1 : 1;

						if (dx < d && dy < d) { // 충돌 발생
							cout << i << "번째 플랫폼에 버블 충돌 발생" << endl;
							bubble.handleCollision(center_p, platform.getWidth() / 2);
						}
					}
				}
				// 플랫폼 - 플레이어간 충돌
				/*
				if (player.isMoving() || player.isJumping() || player.isFalling()) {
					player.moveX();
					handleCollision(player, platform, i);
					player.moveY();
					handleCollision(player, platform, i);
				}
				*/
				if (!findBottom && (!player.isJumping() || (player.isJumping() && player.getVelocity()[1] < 0)) && bottomDetector(player, platform, i)) {
					PlayerIsOnPlatform = true;
					findBottom = true;
				}
				i += 1;
			}
			if (findBottom) {
				player.setVerticalState(Player::STOPV);
			}
			else if (!findBottom && !player.isJumping() && !player.isFalling()) {
				PlayerIsOnPlatform = false;
				player.setVerticalState(Player::FALL);
			}

			if (bPressLeft || bPressRight) {

				player.setFace(bPressLeft ? Player::LEFT : Player::RIGHT);
				player.setHorizontalState(player.HORIZONTAL_STATE::MOVE);
			}
			else player.setHorizontalState(Player::STOPH);
		}
		/* ▲ 위는 로드 중일때는 실행되지 않음 ▲ */

		// 스테이지 전환
		if (load == LOAD_STAGE) {
			stages[state].move();

			if (stages[state].getFirstTransition() == 0) { // 로드 상태
				load = LOAD_PLAYER;
				player.moveTo(stages[state].getPlayerOrigin(), 20.0f);
			}
		}

		if (load == LOAD_PLAYER) { // 플레이어 로드
			player.move();
			player.mMoveTick();

			if (player.moveFinished()) {
				player.setHorizontalState(Player::STOPH);
				player.setVerticalState(Player::FALL);
				load = NONE;
			}
		}

		if (load == LOAD_ENEMY) { // 적 로드

		}

		/* ▼ 아래는 로드 상태와 상관없이 실행됨 ▼ */
		if (!player.canShootBubble()) { // 버블 재발사 대기시간 제어
			player.mBubbleCooldown();
		}
		/* ▲ 위는 로드 상태와 상관없이 실행됨 ▲ */

		start_t = end_t; // 프레임 제어 끝
	}

	glutPostRedisplay();
}

void display() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (state == BEGIN) {

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

	else if (state != BEGIN) {
		glPushMatrix(); // 화면 전환 효과
		if (stages[state].getFirstTransition()) glTranslatef(0, stages[state].getFirstTransition(), 0);
		if (stages[state].getSecondTransition()) glTranslatef(0, stages[state].getSecondTransition() + WINDOW_HEIGHT, 0); // 화면 전환 끝
		
		stages[state].draw();
		glPopMatrix();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		player.draw();

		//3D 요소들 draw
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glEnable(light.getID());

		light.draw();

		//버블 draw
		for (auto& bubble : bubbles) {
			bubble.draw();
		}
	}


	glDisable(light.getID());
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glutSwapBuffers();
}


void keyboardDown(unsigned char key, int x, int y) {

	// Space 누르면 Player가 bubble 생성하여 bubbles vector에 넣기

	if (key == 32) {

		if (state == BEGIN) {
			state = STAGE1;
			stages[state].startFirstTransition();
			load = LOAD_STAGE;
		}

		else if (state == STAGE1) {
			if (player.canShootBubble()) {
				bubbles.push_back(player.shootBubble());
			}
		}
	}

	/* 이 아래는 디버깅을 위한 코드입니다 */
	switch (key)
	{
	case 'v':
		{
			Vector3f v = player.getVelocity();
			cout << "현재 플레이어의 속도: " << v[0] << ", " << v[1] << ", " << v[2] << endl;
			break;
		}
	case 'c':
		{
			Vector3f c = player.getCenter();
			cout << "현재 플레이어의 중심: " << c[0] << ", " << c[1] << ", " << c[2] << endl;
			break;
		}
	case 'p':
		player.printState();
		break;
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
	glOrtho(-boundaryX * wRatio, boundaryX * wRatio, -boundaryY * hRatio, boundaryY * hRatio, -100.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char** argv) {
	// init GLUT and create Window
	glutInit(&argc, argv);
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

	return 0;
}