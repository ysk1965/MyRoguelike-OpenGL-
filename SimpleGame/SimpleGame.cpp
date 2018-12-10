/*
Copyright 2017 Lee Taek Hee (Korea Polytech University)

This program is free software: you can redistribute it and/or modify
it under the terms of the What The Hell License. Do it plz.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"

#include "ScnMgr.h"
#include "global.h"

#define FORCE 10.f

ScnMgr *scn = NULL;
DWORD prevTime = 0;

bool keyW = false;
bool keyA = false;
bool keyS = false;
bool keyD = false;
bool keySP = false;
int shoot = SHOOT_NONE;

void RenderScene(void)
{
	if (prevTime == 0)
	{
		prevTime = timeGetTime();
	}
	DWORD currTime = timeGetTime();
	DWORD elapseTime = currTime - prevTime;
	prevTime = currTime;
	float eTime = (float)elapseTime / 1000.f;
	
	float fx = 0.f, fy = 0.f, fz = 0.f;
	if (keyW)
		fy += FORCE;
	if (keyA)
		fx -= FORCE;
	if (keyS)
		fy -= FORCE;
	if (keyD)
		fx += FORCE;
	if (keySP)
		fz += FORCE / 2;
	scn->ApplyForce(fx, fy, fz, eTime);

	scn->Update(eTime);
	scn->RenderScene();
	
	glutSwapBuffers();
}

void Idle(void)
{
	RenderScene();
}

void MouseInput(int button, int state, int x, int y)
{
	RenderScene();
}

void KeyDownInput(unsigned char key, int x, int y)
{
	if (key == 'w' || key == 'W')
		keyW = true;
	if (key == 'a' || key == 'A')
		keyA = true;
	if (key == 's' || key == 'S')
		keyS = true;
	if (key == 'd' || key == 'D')
		keyD = true;
	if (key == ' ') 
		keySP = true;
	

	RenderScene();
}

void KeyUpInput(unsigned char key, int x, int y)
{
	if (key == 'w' || key == 'W')
		keyW = false;
	if (key == 'a' || key == 'A')
		keyA = false;
	if (key == 's' || key == 'S')
		keyS = false;
	if (key == 'd' || key == 'D')
		keyD = false;
	if (key == ' ') 
		keySP = false;
	
	RenderScene();
}

void SpecialKeyDownInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		shoot = SHOOT_UP;
		scn->Shoot(shoot);
		break;
	case GLUT_KEY_DOWN:
		shoot = SHOOT_DOWN;
		scn->Shoot(shoot);
		break;
	case GLUT_KEY_LEFT:
		shoot = SHOOT_LEFT;
		scn->Shoot(shoot);
		break;
	case GLUT_KEY_RIGHT:
		shoot = SHOOT_RIGHT;
		scn->Shoot(shoot);
		break;
	}

	RenderScene();
}

void SpecialKeyUpInput(int key, int x, int y)
{
	shoot = SHOOT_NONE;

	RenderScene();
}

int main(int argc, char **argv)
{
	// Initialize GL things
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Game Software Engineering KPU");

	glewInit();
	if (glewIsSupported("GL_VERSION_3_0"))
	{
		std::cout << " GLEW Version is 3.0\n ";
	}
	else
	{
		std::cout << "GLEW 3.0 not supported\n ";
	}

	// Initialize Renderer
	scn = new ScnMgr();

	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyDownInput);
	glutKeyboardUpFunc(KeyUpInput);
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glutMouseFunc(MouseInput);
	glutSpecialFunc(SpecialKeyDownInput);
	glutSpecialUpFunc(SpecialKeyUpInput);

	glutMainLoop();

	delete scn;

    return 0;
}

