#include "stdafx.h"
#include "ScnMgr.h"


ScnMgr::ScnMgr()
{
	for (Object* obj:m_Objecct)
		obj = nullptr;

	renderer = new Renderer(500, 500);
	if (!renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}

	m_Objecct[HERO_ID] = new Object();
	m_Objecct[HERO_ID]->set(0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f);
	m_Objecct[HERO_ID]->set_z(0.5f);
	m_Objecct[HERO_ID]->set_vx(0.f);
	m_Objecct[HERO_ID]->set_vy(0.f);
	m_Objecct[HERO_ID]->set_ax(0.f);
	m_Objecct[HERO_ID]->set_ay(0.f);
	m_Objecct[HERO_ID]->set_mass(1.f);
	m_Objecct[HERO_ID]->set_kind(KIND_HERO);

	texIssac = renderer->CreatePngTexture("issac.png");
	
}


ScnMgr::~ScnMgr()
{
	renderer->DeleteTexture(texIssac);
	delete renderer;
	for (int i = 0; i < MAX_OBJECT; i++)
	{
		DeleteObject(i);
	}
}

void ScnMgr::RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.1f, 0.7f, 1.0f);

	// Renderer Test

	float x, y, z;
	float w, h;
	float r, g, b, a;

	m_Objecct[HERO_ID]->get_pos(&x, &y, &z);
	m_Objecct[HERO_ID]->get_size(&w, &h);
	m_Objecct[HERO_ID]->get_color(&r, &g, &b, &a);

	float newX, newY, newZ, newW, newH;
	newX = x * 100;
	newY = y * 100;
	newZ = z * 100;
	newW = w * 300;
	newH = h * 300;

	renderer->DrawTextureRectHeight(newX, newY, 0, newW, newH, r, g, b, a, texIssac, newZ);

	for (int i = 0; i < MAX_OBJECT; i++)
	{
		if( i == HERO_ID);
		else if (m_Objecct[i] != nullptr)
		{
			m_Objecct[i]->get_pos(&x, &y, &z);
			m_Objecct[i]->get_size(&w, &h);
			m_Objecct[i]->get_color(&r, &g, &b, &a);

			newX = x * 100;
			newY = y * 100;
			newZ = z * 100;
			newW = w * 100;
			newH = h * 100;


			renderer->DrawSolidRect(newX, newY, newZ, newW, newH, r, g, b, a);
		}
	}
}

void ScnMgr::Update(float eTime)
{
	for (int i = 0; i < MAX_OBJECT; i++)
	{
		if (m_Objecct[i] != nullptr)
			m_Objecct[i]->Update(eTime);
	}

	UpdateCollision();
}

void ScnMgr::ApplyForce(float fx, float fy, float eTime)
{
	m_Objecct[HERO_ID]->ApplyForce(fx, fy, eTime);
}

void ScnMgr::Shoot(int direct)
{
	float x, y, z;
	float vx, vy;
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

	m_Objecct[HERO_ID]->get_pos(&x, &y, &z);
	m_Objecct[HERO_ID]->get_vel(&vx, &vy);

	bvX += vx;
	bvY += vy;

	AddObject(x, y, z, bvX, bvY, 0.2f, 0.2f);
}

int ScnMgr::FindEmptySlot()
{
	//find empty slot
	int index = -1;
	for (int i = 0; i < MAX_OBJECT; i++)
	{
		if (m_Objecct[i] == nullptr)
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

void ScnMgr::AddObject(float x, float y, float z, float vx, float vy, float width, float height)
{
	int index = FindEmptySlot();

	if (index != -1)
	{
		m_Objecct[index] = new Object();
		m_Objecct[index]->set(x, y, width, height, 1.f, 1.f, 1.f, 1.f);
		m_Objecct[index]->set_z(z);
		m_Objecct[index]->set_vx(vx);
		m_Objecct[index]->set_vy(vy);
		m_Objecct[index]->set_ax(0);
		m_Objecct[index]->set_ay(0);
	}
}

void ScnMgr::DeleteObject(int id)
{
	if (m_Objecct[id] != nullptr)
	{
		delete m_Objecct[id];
		m_Objecct[id] = nullptr;
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
			if (m_Objecct[i]!=nullptr && m_Objecct[j] != nullptr)
			{
				m_Objecct[i]->get_pos(&a_minX, &a_minY, &z);
				m_Objecct[i]->get_size(&a_MaxX, &a_MaxY);
				a_MaxX += a_minX;
				a_MaxY += a_minY;

				m_Objecct[j]->get_pos(&b_minX, &b_minY, &z);
				m_Objecct[j]->get_size(&b_MaxX, &b_MaxY);
				b_MaxX += b_minX;
				b_MaxY += b_minY;

				if (RRCollision(a_minX, a_MaxX, a_minY, a_MaxY,b_minX, b_MaxX, b_minY, b_MaxY ))
				{
					m_Objecct[i]->set_g(0);
					m_Objecct[i]->set_b(0);
					m_Objecct[j]->set_g(0);
					m_Objecct[j]->set_b(0);
				}
			}
		}
	}
}