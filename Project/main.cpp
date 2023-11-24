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

enum GameState { BEGIN, STAGE1TRANSITION, STAGE1 };

GameState state;

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

// Stage와 Player간의 collision detecting

bool isCollisionDetectedLeft(const Player& player, Platform& platform) {

	if (platform.getPlatformType() == Platform::PLATFORM::LEFT) {

		if (((player.getCenter()[0] - PLAYER_SIZE / 2) - (platform.getCenter()[0] + platform.getWidth() / 2) < 0) && player.getVelocity()[0] < 0) {

			cout << "isCollisionDetectedLeft\n";
			return true;
		}

		else
			return false;
	}

	else
		return false;
}

bool isCollisionDetectedRight(const Player& player, Platform& platform) {

	if (platform.getPlatformType() == Platform::PLATFORM::RIGHT) {

		if (((player.getCenter()[0] + PLAYER_SIZE / 2.f) - (platform.getCenter()[0] - platform.getWidth() / 2.f) > 0) && player.getVelocity()[0] > 0) {

			return true;
		}

		else
			return false;
	}

	else
		return false;
}

bool isCollisionDetectedBottom(const Player& player, Platform& platform) {

	if (platform.getPlatformType() == Platform::PLATFORM::BOTTOM) {

		if ( ( ((player.getCenter()[1] - PLAYER_SIZE / 2.f) - (platform.getCenter()[1] + platform.getWidth() / 2.f)) < 0 ) && player.getVelocity()[1] < 0) {

			return true;
		}

		else
			return false;
	}

	else
		return false;
}

bool isCollisionDetectedMiddle(const Player& player, Platform& platform) {

	if (platform.getPlatformType() == Platform::PLATFORM::MIDDLE) {

		if ( ( ((player.getCenter()[1] - PLAYER_SIZE / 2.f) - (platform.getCenter()[1] + platform.getWidth() / 2.f)) < 0  && ((player.getCenter()[1] - PLAYER_SIZE / 2.f) - (platform.getCenter()[1] + platform.getWidth() / 2.f)) > -10)
			&& ((platform.getCenter()[0] - platform.getWidth() / 2.f <= player.getCenter()[0]) && (player.getCenter()[0] <= platform.getCenter()[0] + platform.getWidth() / 2.f))
			&& player.getVelocity()[1] < 0) {

			return true;
		}

		else
			return false;
	}

	else
		return false;
}

// Stage와 Player간의 collision handling

void handleCollision(Player& player, Platform& platform) {

	if (state == STAGE1 && isCollisionDetectedLeft(player, platform)) {

		Vector3f playerposition(-boundaryX + 50 + PLAYER_SIZE / 2.f, player.getCenter()[1], player.getCenter()[2]);
		player.setCenter(playerposition);
		player.setHorizontalState(Player::HORIZONTAL_STATE::STOPH);
		PlayerIsOnPlatform = false;
	}

	if (state == STAGE1 && isCollisionDetectedRight(player, platform)) {

		Vector3f playerposition(boundaryX - 50 - PLAYER_SIZE / 2.f, player.getCenter()[1], player.getCenter()[2]);
		player.setCenter(playerposition);
		player.setHorizontalState(Player::HORIZONTAL_STATE::STOPH);
		PlayerIsOnPlatform = false;
	}

	if (state == STAGE1 && isCollisionDetectedBottom(player, platform)) {

		Vector3f playerposition(player.getCenter()[0], -boundaryY + 25.f + PLAYER_SIZE / 2.f + 3, player.getCenter()[2]);
		player.setCenter(playerposition);
		player.setVerticalState(Player::VERTICAL_STATE::STOPV);
		PlayerIsOnPlatform = false;
	}

	if (state == STAGE1 && isCollisionDetectedMiddle(player, platform)) {

		Vector3f playerposition(player.getCenter()[0], platform.getCenter()[1] + platform.getWidth() / 2.f + PLAYER_SIZE / 2.f + 3, player.getCenter()[2]);
		player.setCenter(playerposition);
		player.setVerticalState(Player::VERTICAL_STATE::STOPV);
		PlayerIsOnPlatform = true;
	}

	else
		PlayerIsOnPlatform = false;
}

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
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■▣▣      ▣▣▣▣▣▣▣▣▣▣▣▣▣▣      ▣▣■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■■■■■■■■■■■■■■■■■■■■■■■■■■■");

	stage1.setStagePlatform(platformInfo);
	stages.push_back(stage1);
	platformInfo.clear();

	state = BEGIN;

	//Player의 초기 State setting
	player.setVerticalState(Player::VERTICAL_STATE::FALL);
	player.setHorizontalState(Player::HORIZONTAL_STATE::MOVE);
}

void idle() {

	end_t = clock();

	if ((float)(end_t - start_t) > 1000 / 30.0f) { // 프레임 제어

		// 플레이어와 버블 움직임을 업데이트 하는 부분

		player.move();

		for (int i = 0; i < stages[1].getStagePlatform().size(); ++i) {
			handleCollision(player, stages[1].getStagePlatform()[i]);
		}

		for (int i = 0; i < bubbles.size(); ++i) {
			bubbles[i].move();
		}

		if (bPressLeft) {

			player.setHorizontalState(player.HORIZONTAL_STATE::MOVE);
			Vector3f velocityleft(-3, player.getVelocity()[1], player.getVelocity()[2]);
			player.setVelocity(velocityleft);
			player.setFace(Player::FACE::LEFT);

			if (!PlayerIsOnPlatform)
				player.setVerticalState(player.VERTICAL_STATE::FALL);
		}

		if (bPressRight) {

			player.setHorizontalState(player.HORIZONTAL_STATE::MOVE);
			Vector3f velocityright(3, player.getVelocity()[1], player.getVelocity()[2]);
			player.setVelocity(velocityright);
			player.setFace(Player::FACE::RIGHT);

			if (!PlayerIsOnPlatform)
				player.setVerticalState(player.VERTICAL_STATE::FALL);
		}

		if (bPressUp) {

			Vector3f velocityjump(player.getVelocity()[0], 3, player.getVelocity()[2]);
			player.setVerticalState(player.VERTICAL_STATE::JUMP);
			player.setVelocity(velocityjump);
		}

		if (bPressDown) {

			player.setVerticalState(player.VERTICAL_STATE::FALL);
		}

		// 충돌 제어
		int i = 0;
		for (auto& platform : stages[1].getStagePlatform()) {
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
			i += 1;
			// 플랫폼 - 플레이어간 충돌
			// todo
		}

		// 스테이지 전환
		if (state == STAGE1TRANSITION) {

			if (stages[1].getFirstTransition()) {
				stages[1].move();
			}
		}
		

		if (!player.canShootBubble()) { // 버블 재발사 대기시간 제어
			player.mBubbleCooldown();
		}

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

	else if (state == STAGE1TRANSITION) {

		bPressLeft = true;

		glPushMatrix(); // 화면 전환 효과
		if (stages[1].getFirstTransition()) glTranslatef(0, stages[1].getFirstTransition(), 0);
		stages[1].draw();

		if (stages[1].getFirstTransition() == 0) {
			bPressLeft = false;  state = STAGE1;
		}

		glPopMatrix();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		player.draw();

	}

	else if (state == STAGE1) {
		//2D 요소들 draw
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		stages[1].draw();
		player.draw();

		//3D 요소들 draw
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glEnable(light.getID());

		light.setAmbient(0.5f, 0.5f, 0.5f, 1.0f);
		light.setDiffuse(0.7f, 0.7f, 0.7f, 1.0f);
		light.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);

		light.draw();

		//버블 draw
		for (int i = 0; i < bubbles.size(); ++i) {
			bubbles[i].draw();
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
			state = STAGE1TRANSITION;
			stages[1].startFirstTransition();
		}

		else if (state == STAGE1) {
			if (player.canShootBubble()) {
				bubbles.push_back(player.shootBubble());
			}
		}
	}
}

void specialKeyDown(int key, int x, int y) {

	// 방향키 눌릴 때 Player가 움직이는 상태로 지정하여 속도만큼 위치를 업데이트할 수 있도록 (업데이트 여부는 Player의 move 함수에 구문 있음)

	if (key == GLUT_KEY_LEFT) {

		bPressLeft = true;
	}

	if (key == GLUT_KEY_RIGHT) {

		bPressRight = true;
	}

	if (key == GLUT_KEY_UP) {

		bPressUp = true;
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
		player.setHorizontalState(player.HORIZONTAL_STATE::STOPH);
		break;

	case GLUT_KEY_RIGHT:

		bPressRight = false;
		player.setHorizontalState(player.HORIZONTAL_STATE::STOPH);
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