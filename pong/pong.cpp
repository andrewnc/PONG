// pong.cpp : Defines the entry point for the console application.

// Known bugs: at a certain speed the collision is no long effective and the opponent will score because the ball will go through your paddle before 
// it recognizes the collision. 

#include "stdafx.h"

#include <string>
#include <windows.h>
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <sstream> 
#include <math.h> 
#include <gl\gl.h>
#include <gl\glu.h>
#include "GL/freeglut.h"

#pragma region variables

#pragma comment(lib, "OpenGL32.lib")

#define VK_W 0x57
#define VK_S 0x53

int width = 500;
int height = 200;
int interval = 1000 / 60;

int multiPlayer = 1;
int levelAI = 3;

int score_left = 0;
int score_right = 0;
bool reset = true;

int racket_width = 10;
int racket_height = 80;
int racket_speed = 3;

float racket_left_x = 5.0f;
float racket_left_y = 50.0f;

float racket_right_x = width - racket_width - 12;
float racket_right_y = 50.0f;

float ball_pos_x = width / 2;
float ball_pos_y = height / 2;
float ball_dir_x = -1.0f;
float ball_dir_y = 0.0f;
int ball_size = 8;
float ball_speed = 2.0f;
#pragma endregion Here are all the globals

std::string int2str(int x){
	std::stringstream ss;
	ss << x;
	return ss.str();
};

std::string float2str(float x){
	std::stringstream ss;
	ss << std::setprecision(3) << x;
	return ss.str();
}

void keyBoard() {
	if (racket_left_y > 0){
		if (GetAsyncKeyState(VK_S)) {
			racket_left_y -= racket_speed;
		}
	}
	else{
		if (GetAsyncKeyState(VK_S)) {
			racket_left_y -= 0;
		}
	}
	if (racket_left_y < height - racket_height){
		if (GetAsyncKeyState(VK_W)) {
			racket_left_y += racket_speed;
		}
	}
	else{
		if (GetAsyncKeyState(VK_W)) {
			racket_left_y += 0;
		}
	}
	if (multiPlayer == 1){
		if (racket_right_y > 0){
			if (GetAsyncKeyState(VK_DOWN))
			{
				racket_right_y -= racket_speed;
			}
		}
		else{
			if (GetAsyncKeyState(VK_DOWN))
			{
				racket_right_y -= 0;
			}
		}
		if (racket_right_y < height - racket_height){
			if (GetAsyncKeyState(VK_UP)) {
				racket_right_y += racket_speed;
			}
		}
		else{
			if (GetAsyncKeyState(VK_UP)) {
				racket_right_y += 0;
			}
		}
	}

}

void drawRect(float x, float y, float width, float height){
	glBegin(GL_QUADS);
	glVertex2f(x,y);
	glVertex2f(x + width, y);
	glVertex2f(x+width, y+height);
	glVertex2f(x, y+height);
	glEnd();
}

void drawText(float x, float y, std::string text){
	glRasterPos2f(x, y);
	glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char*)text.c_str());
}

void draw(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	drawRect(racket_left_x, racket_left_y, racket_width, racket_height);
	drawRect(racket_right_x, racket_right_y, racket_width, racket_height);

	drawRect(ball_pos_x - ball_size / 2, ball_pos_y - ball_size / 2, ball_size, ball_size);

	drawText(width / 2 - 10, height - 25, int2str(score_left) + ":" + int2str(score_right));
	drawText(width / 2 + 100, height - 25, "x: " + float2str(ball_pos_x) + "\ny: " + float2str(ball_pos_y) + "\nspeed: " + float2str(ball_speed));

	glutSwapBuffers();
}

#pragma region Artificial Intellegences
void derpAI(){
	if (racket_right_y < height - racket_height){
		if (ball_dir_y > 0.0f && ball_dir_x > 0.0f){
			racket_right_y += racket_speed;
		}
	}
	else{
		racket_right_y += 0;
	}
	if (racket_right_y > 0){
		if (ball_dir_y < 0.0f && ball_dir_x > 0.0f){
			racket_right_y -= racket_speed;
		}
	}
	else{
		racket_right_y -= 0;
	}

}

void rightDumbAI(){
	if ((racket_right_y < height - racket_height)){
		if (ball_dir_y > 0.0f){
			racket_right_y += ball_dir_y;
		}
	}
	else{
		racket_right_y += 0;
	}
	if ((racket_right_y > 0)){
		if (ball_dir_y < 0.0f){
			racket_right_y += ball_dir_y;
		}
	}
	else{
		racket_right_y -= 0;
	}

}

void rightSmartAI(){
	if ((racket_right_y < height - racket_height) && (ball_pos_x <= 350)){
		if (ball_dir_y > 0.0f){
			racket_right_y += ball_dir_y*racket_speed;
		}
	}
	else{
		racket_right_y += 0;
	}
	if ((racket_right_y > 0) && (ball_pos_x <= 350)){
		if (ball_dir_y < 0.0f){
			racket_right_y += ball_dir_y*racket_speed;
		}
	}
	else{
		racket_right_y -= 0;
	}

}


void rightSmartestAI(){
	int topORBot = rand() % 5;
	if (topORBot = 0){
		racket_right_y = ball_pos_y - (racket_height / 2);
	}
	else if (topORBot = 1){
		racket_right_y = ball_pos_y - ((racket_height / 2) + 10);
	}
	else if (topORBot = 2){
		racket_right_y = ball_pos_y - ((racket_height / 2) - 10);
	}
	else if (topORBot = 3){
		racket_right_y = ball_pos_y - ((racket_height / 2) + 50);
	}
	else if (topORBot = 4){
		racket_right_y = ball_pos_y - ((racket_height / 2) - 50);
	}

}

#pragma endregion AI for single player mode

void updateBall(){
	if (reset){
		ball_speed = 2.0f;
		racket_left_y = 50.0f;
		racket_right_y = 50.0f;
		reset = false;
	}
	ball_pos_x += ball_dir_x * ball_speed;
	ball_pos_y += ball_dir_y * ball_speed;

	// hit by left racket?
	if (ball_pos_x < racket_left_x + racket_width &&
		ball_pos_x > racket_left_x &&
		ball_pos_y < racket_left_y + racket_height &&
		ball_pos_y > racket_left_y) {
		// set fly direction depending on where it hit the racket
		// (t is 0.5 if hit at top, 0 at center, -0.5 at bottom)
		float t = ((ball_pos_y - racket_left_y) / racket_height) - 0.5f;
		ball_dir_x = fabs(ball_dir_x); // force it to be positive
		ball_dir_y = t;
	}

	// hit by right racket?
	if (ball_pos_x > racket_right_x &&
		ball_pos_x < racket_right_x + racket_width &&
		ball_pos_y < racket_right_y + racket_height &&
		ball_pos_y > racket_right_y) {
		// set fly direction depending on where it hit the racket
		// (t is 0.5 if hit at top, 0 at center, -0.5 at bottom)
		float t = ((ball_pos_y - racket_right_y) / racket_height) - 0.5f;
		ball_dir_x = -fabs(ball_dir_x); // force it to be negative
		ball_dir_y = t;
		ball_speed += 0.5f;
	}

	if (ball_pos_x < 0){
		++score_right; 
		ball_pos_x = width / 2;
		ball_pos_y = height / 2;
		ball_dir_x = fabs(ball_dir_x);
		ball_dir_y = 0;
		reset = true;

	}
	
	if (ball_pos_x > width) {
		++score_left;
		ball_pos_x = width / 2;
		ball_pos_y = height / 2;
		ball_dir_x = -fabs(ball_dir_x); 
		ball_dir_y = 0;
		reset = true;
	}

	
	if (ball_pos_y > height - 15) {
		ball_dir_y = -fabs(ball_dir_y);
	}

	
	if (ball_pos_y < 0) {
		ball_dir_y = fabs(ball_dir_y); 
	}



}

void update(int value){

	keyBoard();

	if(multiPlayer == 2){
		if (levelAI == 1){
			rightDumbAI();
		}
		else if (levelAI == 2){
			rightSmartAI();
		}
		else if (levelAI == 3){
			rightSmartestAI();
		}
		else if (levelAI == 4){
			derpAI();
		}
	}

	updateBall();

	glutTimerFunc(interval, update, 0);

	glutPostRedisplay();
}

void enable2D(int width, int height){
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, width, 0.0f, height, 0.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int _tmain(int argc, char** argv)
{
	std::cout << "WELCOME TO PONG!" << std::endl;
	std::cout << "Multiplayer(1) or Singleplayer(2): ";
	std::cin >> multiPlayer;

	if (multiPlayer == 2){
		std::cout << "Choose a difficulty level: ";
		std::cin >> levelAI;
	}


	char * a = "pong";
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutCreateWindow(a);

	glutDisplayFunc(draw);
	glutTimerFunc(interval, update, 0);

	enable2D(width, height);

	glColor3f(1.0f, 1.0f, 1.0f);

	glutMainLoop();

	return 0;
}

