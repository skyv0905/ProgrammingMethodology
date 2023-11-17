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

enum GameState { BEGIN, STAGE1 };

GameState state;

vector<Stage> stages;
vector<Bubble> bubbles;
vector<Texture> textures;

bool bPressLeft;
bool bPressRight;
bool bPressUp;
bool bPressDown;

Player player(-boundaryX + 56 + PLAYER_SIZE * 0.5f, -boundaryY + 25 + PLAYER_SIZE * 0.5f, 0.0f, PLAYER_SIZE);
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

	// MAIN
	Stage main(0);
	stages.push_back(main);

	// STAGE 1
	// Platform 생성
	Stage stage1(1);
	stage1.setStagePlatformTextureID(textures[1].getTextureID(), textures[2].getTextureID(), textures[3].getTextureID());
	vector<string> platformInfo;
	platformInfo.push_back("■■    ■■■■■■■■■■■■■■■■■■■■■■■■");
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
	platformInfo.push_back("■■▣▣    ▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣    ▣▣■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■▣▣    ▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣    ▣▣■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■▣▣    ▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣    ▣▣■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■                                                ■■");
	platformInfo.push_back("■■■■■■■■■■■■■■■■■■■■■■■■■■■■");

	stage1.setStagePlatform(platformInfo);
	stages.push_back(stage1);
	platformInfo.clear();

	state = BEGIN;
}



void idle() {

	end_t = clock();

	Vector3f velocityright(3, 0, 0);
	Vector3f velocityleft(-3, 0, 0);
	Vector3f velocitystop(0, 0, 0);
	Vector3f velocityup(0, 10, 0);
	Vector3f velocitydown(0, -10, 0);


	// 플레이어가 바라보는 방향에 따라 속도 부호 다르게 만드는 부분

	if (bPressLeft) {

		if (bPressUp)
			player.setVelocity(velocityleft + velocityup);
		else if (bPressDown)
			player.setVelocity(velocityleft + velocitydown);
		else
			player.setVelocity(velocityleft);

		player.setFace(player.FACE::LEFT);
	}

	else if (bPressRight) {

		if (bPressUp)
			player.setVelocity(velocityright + velocityup);
		else if (bPressDown)
			player.setVelocity(velocityright + velocitydown);
		else
			player.setVelocity(velocityright);

		player.setFace(player.FACE::RIGHT);
	}

	else if (bPressUp) {

		player.setVelocity(velocityup);
	}

	else if (bPressDown) {

		player.setVelocity(velocitydown);
	}

	if ((float)(end_t - start_t) > 1000 / 30.0f) { // 프레임 제어

		// 플레이어와 버블 움직임을 업데이트 하는 부분
		player.move();

		for (int i = 0; i < bubbles.size(); ++i) {
			bubbles[i].move();
		}

		// 충돌 제어
		int i = 0;
		for (auto& platform : stages[state].getStagePlatform()) {
			// 플랫폼 - 버블간 충돌
			if (platform.getPlatformType() == Platform::PLATFORM::GROUND) {
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
		if (stages[state].getFirstTransition() || stages[state].getSecondTransition()) {
			stages[state].move();
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


	if (state != BEGIN) {
		//2D 요소들 draw
		glPushMatrix(); // 화면 전환 효과
		if (stages[state].getFirstTransition()) glTranslatef(0, stages[state].getFirstTransition(), 0);
		if (stages[state].getSecondTransition()) glTranslatef(0, stages[state].getSecondTransition() + WINDOW_HEIGHT, 0); // 화면 전환 끝
		stages[state].draw();
		glPopMatrix();

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
			state = STAGE1;
			stages[state].startFirstTransition();
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
		player.setHorizontalState(player.HORIZONTAL_STATE::MOVE);
	}

	if (key == GLUT_KEY_RIGHT) {

		bPressRight = true;
		player.setHorizontalState(player.HORIZONTAL_STATE::MOVE);
	}

	if (key == GLUT_KEY_UP) {

		bPressUp = true;
		player.setVerticalState(player.VERTICAL_STATE::JUMP);
	}

	if (key == GLUT_KEY_DOWN) {

		bPressDown = true;
		player.setVerticalState(player.VERTICAL_STATE::FALL);
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
		player.setVerticalState(player.VERTICAL_STATE::STOPV);
		break;

	case GLUT_KEY_DOWN:

		bPressDown = false;
		player.setVerticalState(player.VERTICAL_STATE::STOPV);
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