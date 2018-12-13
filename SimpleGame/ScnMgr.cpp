#include "stdafx.h"
#include "ScnMgr.h"


ScnMgr::ScnMgr()
{
	for (Object* obj:m_Object)
		obj = nullptr;

	m_Renderer = new Renderer(500, 500);
	if (!m_Renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}

	m_Sound = new Sound();
	m_SoundBG = m_Sound->CreateSound("./Sound/Sketch3.wav");
	m_SoundFire = m_Sound->CreateSound("./Sound/Shoot.wav");
	m_SoundExplosion = m_Sound->CreateSound("./Sound/Explosion.wav");

	m_Sound->PlaySound(m_SoundBG, true, 1);

	m_Object[HERO_ID] = new Object();
	m_Object[HERO_ID]->set(0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f);
	m_Object[HERO_ID]->set_z(0.0f);
	m_Object[HERO_ID]->set_vx(0.f);
	m_Object[HERO_ID]->set_vy(0.f);
	m_Object[HERO_ID]->set_vz(0.f);
	m_Object[HERO_ID]->set_ax(0.f);
	m_Object[HERO_ID]->set_ay(0.f);
	m_Object[HERO_ID]->set_az(0.f);
	m_Object[HERO_ID]->set_mass(1.f);
	m_Object[HERO_ID]->set_kind(KIND_HERO);

	m_texIssac = m_Renderer->CreatePngTexture("issac.png");

	// Add Building
	AddObject(1.f, 0.f, 0.f, 0.f, 0.f, 0.5f, 0.5f, KIND_BUILDING, 10, STATE_GROUND); // 수정필요
}


ScnMgr::~ScnMgr()
{
	m_Renderer->DeleteTexture(m_texIssac);
	delete m_Renderer;
	for (int i = 0; i < MAX_OBJECT; i++)
	{
		DeleteObject(i);
	}
}

void ScnMgr::RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.3f, 0.1f, 0.2f, 1.0f);

	// Renderer Test

	float x, y, z;
	float w, h;
	float r, g, b, a;
	int state = STATE_GROUND;

	m_Object[HERO_ID]->get_pos(&x, &y, &z);
	m_Object[HERO_ID]->get_size(&w, &h);
	m_Object[HERO_ID]->get_color(&r, &g, &b, &a);
	m_Object[HERO_ID]->get_state(&state);

	float newX, newY, newZ, newW, newH;
	newX = x * 100;
	newY = y * 100;
	newZ = z * 100;
	newW = w * 300;
	newH = h * 300;

	m_Renderer->DrawTextureRectHeight(newX, newY, 0, newW, newH, r, g, b, a, m_texIssac, newZ);

	for (int i = 0; i < MAX_OBJECT; i++)
	{
		if( i == HERO_ID);
		else if (m_Object[i] != nullptr)
		{
			m_Object[i]->get_pos(&x, &y, &z);
			m_Object[i]->get_size(&w, &h);
			m_Object[i]->get_color(&r, &g, &b, &a);

			newX = x * 100;
			newY = y * 100;
			newZ = z * 100;

			newW = w * 100;
			newH = h * 100;

			m_Renderer->DrawSolidRect(newX, newY, newZ, newW, newH, r, g, b, a);
		}
	}
}

void ScnMgr::Update(float eTime)
{
	for (int i = 0; i < MAX_OBJECT; i++)
	{
		if (m_Object[i] != nullptr)
			m_Object[i]->Update(eTime);
	}

	UpdateCollision();
	DoGarbageCollect();
}

void ScnMgr::ApplyForce(float fx, float fy, float fz, float eTime)
{
	int state;
	float pos_z;
	m_Object[HERO_ID]->get_state(&state);
	m_Object[HERO_ID]->get_posZ(&pos_z);
	if (state == STATE_AIR) {
		fz = 0;
	}
	else {
		m_Object[HERO_ID]->set_z(pos_z += fz / 5);
	}

	m_Object[HERO_ID]->ApplyForce(fx, fy, fz, eTime);
}

void ScnMgr::Shoot(int direct)
{
	float x, y, z;
	float vx, vy, vz;
	float bvX = 0.f, bvY = 0.f;
	float amount = 5.f;
	
	if (direct == SHOOT_UP)
		bvY += amount;
	else if (direct == SHOOT_DOWN)
		bvY -= amount;
	else if (direct == SHOOT_LEFT)
		bvX -= amount;
	else if (direct == SHOOT_RIGHT)
		bvX += amount;

	m_Object[HERO_ID]->get_pos(&x, &y, &z);
	m_Object[HERO_ID]->get_vel(&vx, &vy, &vz);

	bvX += vx;
	bvY += vy;

	AddObject(x, y, z, bvX, bvY, 0.2f, 0.2f, KIND_BULLET, 1, STATE_GROUND);
	m_Sound->PlaySound(m_SoundFire, false, 1);
}

int ScnMgr::FindEmptySlot()
{
	//find empty slot
	int index = -1;
	for (int i = 0; i < MAX_OBJECT; i++)
	{
		if (m_Object[i] == nullptr)
		{
			index = i;
			break;
		}
	}
	if (index == -1)
	{
		std::cout << "all slos are occupied." << std::endl;
	}
	return index;
}

void ScnMgr::AddObject(float x, float y, float z, float vx, float vy, float m_width, float m_height, int m_kind, int health, int state)
{
	int index = FindEmptySlot();

	if (index != -1)
	{
		m_Object[index] = new Object();
		m_Object[index]->set(x, y, m_width, m_height, 1.f, 1.f, 1.f, 1.f);
		m_Object[index]->set_z(z);
		m_Object[index]->set_vx(vx);
		m_Object[index]->set_vy(vy);
		m_Object[index]->set_ax(0);
		m_Object[index]->set_ay(0);
		m_Object[index]->set_kind(m_kind);
		m_Object[index]->set_health(1);
		m_Object[index]->set_state(state);
	}
	//cout << m_kind << endl;
}

void ScnMgr::DeleteObject(int id)
{
	if (m_Object[id] != nullptr)
	{
		delete m_Object[id];
		m_Object[id] = nullptr;
	}
}

void ScnMgr::DoGarbageCollect()
{
	for (int i = 0; i < MAX_OBJECT; i++) {
		if (m_Object[i] == NULL) continue;

		int health, m_kind;
		float vel, vx, vy, vz, px, py, pz;
		m_Object[i]->get_kind(&m_kind);
		m_Object[i]->get_health(&health);
		m_Object[i]->get_vel(&vx, &vy, &vz);
		m_Object[i]->get_pos(&px, &py, &pz);

		vel = sqrtf(vx * vx + vy * vy + vz * vz);

		// Check Velocity
		if (vel < FLT_EPSILON && m_kind == KIND_BULLET) {
			cout << "Check Velocity - " << i << endl;
			DeleteObject(i);
			continue;
		}

		// Check Health
		if (health <= 0 && (m_kind == KIND_BULLET || m_kind == KIND_BUILDING)) {
			cout << "Check Health - " << i << endl;
			DeleteObject(i);
			continue;
		}

		// Check Outofbounce
		if (px < -2.5f || px > 2.5f || py < -2.5f || py > 2.5f) {
			if (m_kind == KIND_BULLET) {
				cout << "Check Outofbounce - " << i << endl;
				DeleteObject(i);
				continue;
			}
		}
	}
}


bool ScnMgr::RRCollision(float a_minX, float a_maxX, float a_minY, float a_maxY, float b_minX, float b_maxX, float b_minY, float b_maxY)
{
	if (a_minX > b_maxX)
		return false;
	if (a_maxX < b_minX)
		return false;
	if (a_minY > b_maxY)
		return false;
	if (a_maxY < b_minY)
		return false;
	return true;
}

void ScnMgr::UpdateCollision()
{
	float a_minX, a_MaxX, a_minY, a_MaxY;
	float b_minX, b_MaxX, b_minY, b_MaxY;
	float z; 

	for (int i = HERO_ID + 1; i < MAX_OBJECT; i++)
	{
		for(int j = i + 1; j<MAX_OBJECT; j++)
		{
			if (m_Object[i]!=nullptr && m_Object[j] != nullptr)
			{
				m_Object[i]->get_pos(&a_minX, &a_minY, &z);
				m_Object[i]->get_size(&a_MaxX, &a_MaxY);
				a_MaxX += a_minX;
				a_MaxY += a_minY;

				m_Object[j]->get_pos(&b_minX, &b_minY, &z);
				m_Object[j]->get_size(&b_MaxX, &b_MaxY);
				b_MaxX += b_minX;
				b_MaxY += b_minY;

				if (RRCollision(a_minX, a_MaxX, a_minY, a_MaxY,b_minX, b_MaxX, b_minY, b_MaxY ))
				{
					//m_Object[i]->set_r(0);
					//m_Object[i]->set_b(0);
					//m_Object[j]->set_g(0);
					//m_Object[j]->set_b(0);
					int isBuilding, iHealth;
					m_Object[i]->get_kind(&isBuilding);
					m_Object[i]->get_health(&iHealth);

					if (isBuilding == KIND_BUILDING) {
						//cout << "Building Check" << endl;
						m_Object[i]->set_r(0.1*(rand() % 10));
						m_Object[i]->set_g(0.1*(rand() % 10));
						m_Object[i]->set_b(0.1*(rand() % 10));
					}
					m_Object[j]->set_health(--iHealth);
					cout << iHealth << endl;
				}
			}
		}
	}
}