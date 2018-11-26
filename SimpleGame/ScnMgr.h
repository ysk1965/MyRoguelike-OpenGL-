#pragma once

#include "Renderer.h"
#include "Object.h"
#include "global.h"

class ScnMgr
{
	Renderer *renderer = NULL;
	int rad = 0;
	int oldTime;
	GLuint texIssac;

	Object *m_Objecct[MAX_OBJECT] = { nullptr };

	int FindEmptySlot();
		
public:
	ScnMgr();
	~ScnMgr();

	void RenderScene();

	void Update(float eTime);
	
	void UpdateCollision();
	bool RRCollision(float a_minX, float a_maxX, float a_minY, float a_maxY, float b_minX, float b_maxX, float b_minY, float b_maxY);

	void ApplyForce(float fx, float fy, float eTime);
	void Shoot(int direct);

	void AddObject(float x, float y, float z, float vx, float vy, float width, float height);
	void DeleteObject(int id);
};

