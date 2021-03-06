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
	GLuint m_clearUI;

	Sound *m_Sound = NULL;
	int m_SoundBG = 0;
	int m_SoundFire = 0;
	int m_SoundHplose = 0;
	int m_SoundHit = 0;
	int m_SoundDeleteCard = 0;
	int m_SoundStart = 0;
	int m_SoundLevelup = 0;
	int m_SoundHitmiss = 0;
	int m_SoundDie = 0;
	int m_SoundItem = 0;
	int m_SoundBomb = 0;

	bool bfrequency = true;
	Object *m_Object[MAX_OBJECT] = { nullptr };

	int FindEmptySlot();

	float m_curX = 0.f;
	float m_curY = 0.f;

	int m_attackcard = 0;
	int killscore = 0;

	float event_time = 0.f;

	bool isUIScene = true;
	bool isClearScene = false;
	bool isHit = false;
	bool isEvent = false;
	
	//level
	int current_level = 0;
	float level_curX = 3.f;
	float spawnfrequency = 0.f;
	float level_speed = 0.1f;
	float m_playersize = 1.0f;
	int level_frequency = 4;
public:
	ScnMgr();
	~ScnMgr();

	void UIScene();

	void RenderScene(float eTime);

	void Update(float eTime);
	
	void UpdateCollision();
	bool RRCollision(float a_minX, float a_maxX, float a_minY, float a_maxY, float b_minX, float b_maxX, float b_minY, float b_maxY);
	void CardSpawn();
	void EventCardSpawn();

	void ApplyForce(float fx, float fy, float fz, float eTime);
	void Shoot(int direct);
	void ShootChange(int change);
	void SpaceBarChange();

	void AddObject(float x, float y, float z, float vx, float vy, float m_width, float m_height, int m_kind, int health, int state);
	void Particle(float x, float y, float z, float m_width, float m_height, int m_kind, int health, int state);
	void DeleteObject(int id);

	void DoGarbageCollect();

	void SceneInit();
	void SceneChange();
};

