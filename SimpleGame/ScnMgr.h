#pragma once

#include "Renderer.h"
#include "Object.h"
#include "global.h"
#include "Sound.h"

class ScnMgr
{
	Renderer *m_Renderer = NULL;
	int m_rad = 0;
	int m_oldTime;
	GLuint m_mainUI;
	GLuint m_texExplosion;
	GLuint m_texMonster;
	GLuint m_texCard;

	Sound *m_Sound = NULL;
	int m_SoundBG = 0;
	int m_SoundFire = 0;
	int m_SoundExplosion = 0;
	int m_SoundHit = 0;
	int m_SoundDeleteCard = 0;

	bool bfrequency = true;
	Object *m_Object[MAX_OBJECT] = { nullptr };

	int FindEmptySlot();

	float m_curX = 0.f;
	float m_curY = 0.f;

	int m_attackcard = 0;
	int killscore = 0;
	
	float spawnfrequency = 0.f;

	bool isUIScene = true;
public:
	ScnMgr();
	~ScnMgr();

	void UIScene();

	void RenderScene();

	void Update(float eTime);
	
	void UpdateCollision();
	bool RRCollision(float a_minX, float a_maxX, float a_minY, float a_maxY, float b_minX, float b_maxX, float b_minY, float b_maxY);
	void CardSpawn();

	void ApplyForce(float fx, float fy, float fz, float eTime);
	void Shoot(int direct);
	void ShootChange(int change);
	void SpaceBarChange();

	void AddObject(float x, float y, float z, float vx, float vy, float m_width, float m_height, int m_kind, int health, int state);
	void DeleteObject(int id);

	void DoGarbageCollect();

	void SceneInit();
	void SceneChange();
};

