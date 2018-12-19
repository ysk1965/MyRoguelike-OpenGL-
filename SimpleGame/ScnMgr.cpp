#include "stdafx.h"
#include "ScnMgr.h"


ScnMgr::ScnMgr()
{
	for (Object* obj:m_Object)
		obj = nullptr;

	m_Renderer = new Renderer(RENDERER_W, RENDERER_H);
	if (!m_Renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}

	m_Sound = new Sound();
	m_SoundBG = m_Sound->CreateSound("./Sound/Sketch3.wav");
	m_SoundFire = m_Sound->CreateSound("./Sound/Shoot.wav");
	m_SoundExplosion = m_Sound->CreateSound("./Sound/Explosion.wav");
	m_SoundHit = m_Sound->CreateSound("./Sound/Hit.wav");
	m_SoundDeleteCard = m_Sound->CreateSound("./Sound/DeleteCard.wav");

	m_Sound->PlaySound(m_SoundBG, true, 1);

	m_Object[HERO_ID] = new Object();
	m_Object[HERO_ID]->set(0.f, 0.f, 0.5f, 0.5f, 1.f, 1.f, 1.f, 1.f);
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
	m_texExplosion = m_Renderer->CreatePngTexture("explosion.png");
	m_texMonster = m_Renderer->CreatePngTexture("monster.png");
	m_texCard = m_Renderer->CreatePngTexture("card.png");

	// Add Building
	AddObject(1.f, 0.f, 0.f, 0.f, 0.f, 0.5f, 0.5f, KIND_BUILDING, 10, STATE_GROUND); // 수정필요
	//AddObject(4.8f, 2.7f, 0.f, 0.f, 0.f, 1.0f, 1.0f, KIND_UI, 10, STATE_GROUND); // 수정필요
}


ScnMgr::~ScnMgr()
{
	m_Renderer->DeleteTexture(m_texIssac);
	m_Renderer->DeleteTexture(m_texExplosion);
	m_Renderer->DeleteTexture(m_texMonster);
	m_Renderer->DeleteTexture(m_texCard);
	delete m_Renderer;
	for (int i = 0; i < MAX_OBJECT; i++)
	{
		DeleteObject(i);
	}
}

void ScnMgr::UIScene() {
	// 내가 갖고 있는 카드
	m_Renderer->DrawTextureRectSeqXY((RENDERER_W/2) - 100, -(RENDERER_H/2) + 100, 1.f,
		130.f, 130.f,
		1.f, 1.f, 1.f, 1.f
		, m_texCard, m_curX, m_attackcard, 14.0f, 5.0f);
}

void ScnMgr::RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(killscore / 100.f, killscore / 100.f, killscore / 100.f, 1.0f);

	UIScene();
	// Renderer Test

	float x, y, z;
	float w, h;
	float r, g, b, a;
	float tx, ty;
	int state = STATE_GROUND;
	int kind;

	m_Object[HERO_ID]->get_pos(&x, &y, &z);
	m_Object[HERO_ID]->get_size(&w, &h);
	m_Object[HERO_ID]->get_color(&r, &g, &b, &a);
	m_Object[HERO_ID]->get_state(&state);

	float newX, newY, newZ, newW, newH;
	newX = x * 100;
	newY = y * 100;
	newZ = z * 1;
	newW = w * 200;
	newH = h * 200;

	//m_Renderer->DrawTextureRectHeight(newX, newY, 0, newW, newH, r, g, b, a, m_texIssac, newZ);
	m_Renderer->DrawTextureRectSeqXY(newX, newY, 0, newW, newH, r, g, b, a, m_texMonster, m_curX, m_curY, 5.0f, 5.0f);
	//
	//m_Renderer->DrawTextureRectSeqXY(newX, newY, 0, newW/2, newH/2, r, g, b, a, m_texExplosion, m_curX, m_curY, 9.0f, 9.0f);


	for (int i = 0; i < MAX_OBJECT; i++)
	{
		if( i == HERO_ID);
		else if (m_Object[i] != nullptr)
		{
			m_Object[i]->get_pos(&x, &y, &z);
			m_Object[i]->get_size(&w, &h);
			m_Object[i]->get_color(&r, &g, &b, &a);
			m_Object[i]->get_kind(&kind);
			m_Object[i]->get_tex(&tx, &ty);

			newX = x * 100;
			newY = y * 100;
			newZ = z * 100;

			newW = w * 100;
			newH = h * 100;

			if (kind == KIND_CARD) {
				m_Renderer->DrawTextureRectSeqXY(newX, newY, 0, newW, newH, r, g, b, a, m_texCard, tx, ty, 14.0f, 5.0f);
			}
			else if (kind == KIND_BUILDING) {
				m_Renderer->DrawSolidRect(newX, newY, newZ, newW, newH, r, g, b, a);
			}
			else if (kind == KIND_BULLET) {
				m_Renderer->DrawTextureRectSeqXY(newX, newY, 0, newW, newH, r, g, b, a, m_texCard, m_curX, m_attackcard, 14.0f, 5.0f);
				//m_Renderer->DrawTextureRectSeqXY(newX, newY, 0, newW, newH, r, g, b, a, m_texExplosion, m_curX, m_curY, 9.0f, 9.0f);
			}
		}
	}
}

void ScnMgr::CardSpawn() {
	float x, y, z;
	float vx, vy, vz;
	float tx, ty;
	float bvX = 0.f, bvY = 0.f;
	int health = 2;
	float hx, hy, hz; // hero

	m_Object[HERO_ID]->get_pos(&hx, &hy, &hz);
	
	while (true) {
		
		x = rand() % 960 * 0.01f - 4.80f;
		y = rand() % 540 * 0.01f - 2.70f;
		z = 0;
		if (sqrt((hx - x)*(hx - x) + (hy - y) * (hy - y)) > 1) {
			break;
		}
	}

	AddObject(x, y, z, bvX, bvY, 0.5f, 0.5f, KIND_CARD, health, STATE_GROUND);
}

void ScnMgr::Update(float eTime)
{
	float object_vx, object_vy, object_vz;
	float object_px, object_py, object_pz;
	float hero_px, hero_py, hero_pz;
	float bullet_sw, bullet_sh;
	int object_kind;
	float amount = 50.f;
	static int spawn_cnt = 0;
	
	if (spawnfrequency > 1.f && bfrequency == true) { // 1이 빈도 수 조절
		spawn_cnt++;
		if (spawn_cnt % 2 == 0) {
			CardSpawn();
		}
		bfrequency = false;
	}

	if (spawnfrequency < -1.f && bfrequency == false) {
		//CardSpawn();
		bfrequency = true;
	}

	if (bfrequency == true) {
		spawnfrequency += eTime*3;
	}
	else {
		spawnfrequency -= eTime*3;
	}

	//cout << spawnfrequency << endl;

	m_Object[HERO_ID]->get_pos(&hero_px, &hero_py, &hero_pz);

	for (int i = 0; i < MAX_OBJECT; i++)
	{
		if (m_Object[i] != nullptr) {
			m_Object[i]->Update(eTime);

			m_Object[i]->get_kind(&object_kind);
			if (object_kind == KIND_CARD) {
				m_Object[i]->get_vel(&object_vx, &object_vy, &object_vz);
				m_Object[i]->get_pos(&object_px, &object_py, &object_pz);

				m_Object[i]->set_vx((hero_px - object_px) * eTime * amount);
				m_Object[i]->set_vy((hero_py - object_py) * eTime * amount);
				m_Object[i]->set_vz((hero_pz - object_pz) * eTime * amount);
			}
			else if (object_kind == KIND_BULLET) {
				m_Object[i]->get_size(&bullet_sw, &bullet_sh);
				m_Object[i]->set_w(0.3 + spawnfrequency/15);
				m_Object[i]->set_h(0.3 + spawnfrequency/15);
			}
 		}
	}

	// SpriteImage
	m_curX += eTime * 10.f;
	if (m_curX > 14.f)
	{
		m_curX = 0.f;
		m_curY += 1.f;
		if (m_curY > 3.f)
		m_curY = 0.f;
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

	AddObject(x, y, z, bvX, bvY, 0.3f, 0.3f, KIND_BULLET, 1, STATE_GROUND);
	m_Sound->PlaySound(m_SoundFire, false, 1);
}

void ScnMgr::ShootChange(int change)
{
	if (change == CARD_SPADE)
		m_attackcard = CARD_SPADE;
	else if (change == CARD_DIAMOND)
		m_attackcard = CARD_DIAMOND;
	else if (change == CARD_HEART)
		m_attackcard = CARD_HEART;
	else if (change == CARD_CLOVER)
		m_attackcard = CARD_CLOVER;
}

void ScnMgr::SpaceBarChange()
{
	if (m_attackcard == CARD_CLOVER) {
		m_attackcard = CARD_SPADE;
	}
	else if (m_attackcard == CARD_SPADE) {
		m_attackcard = CARD_DIAMOND;
	}
	else if (m_attackcard == CARD_DIAMOND) {
		m_attackcard = CARD_HEART;
	}
	else if (m_attackcard == CARD_HEART) {
		m_attackcard = CARD_CLOVER;
	}
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
	float tx, ty;

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
		m_Object[index]->set_state(state);
		m_Object[index]->set_tex(m_curX, m_curY);

		m_Object[index]->get_tex(&tx, &ty);
		if (m_kind == KIND_CARD) {
			m_Object[index]->set_health(tx + 1);
			m_Object[index]->get_health(&health);
			cout << "new Health : " << health << endl;
		}
		else {
			m_Object[index]->set_health(health);
		}
	}

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
		if (health <= 0 && (m_kind == KIND_BULLET || m_kind == KIND_BUILDING || m_kind == KIND_CARD)) {
			cout << "Check Health - " << i << "[" << health << "]" <<endl;
			if (m_kind == KIND_CARD) {
				killscore++;
			}
			DeleteObject(i);
			continue;
		}

		// Check Outofbounce  1000 5 // 960 540 
		if (px < -(RENDERER_W/200.f) || px >(RENDERER_W / 200.f) || py < -(RENDERER_H / 200.f) || py >(RENDERER_H / 200.f)) {
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
	int kind_J, kind_I;
	int health_J, health_I;

	for (int i = HERO_ID + 1; i < MAX_OBJECT; i++)
	{
		for(int j = i + 1; j<MAX_OBJECT; j++)
		{
			if (m_Object[i]!=nullptr && m_Object[j] != nullptr)
			{
				m_Object[i]->get_kind(&kind_I);
				m_Object[i]->get_health(&health_I);
				m_Object[i]->get_pos(&a_minX, &a_minY, &z);
				m_Object[i]->get_size(&a_MaxX, &a_MaxY);
				a_MaxX += a_minX;
				a_MaxY += a_minY;

				m_Object[j]->get_kind(&kind_J);
				m_Object[j]->get_health(&health_J);
				m_Object[j]->get_pos(&b_minX, &b_minY, &z);
				m_Object[j]->get_size(&b_MaxX, &b_MaxY);
				b_MaxX += b_minX;
				b_MaxY += b_minY;


				if (RRCollision(a_minX, a_MaxX, a_minY, a_MaxY,b_minX, b_MaxX, b_minY, b_MaxY ))
				{
					float tx, ty;

					// Bullet 은 i이던 j이던 health가 깍여야 함.
					if (kind_I == KIND_BULLET) {
						m_Object[i]->set_health(--health_I); // bullet

						if (kind_J == KIND_BUILDING) {
							//cout << "Building Check" << endl;
							m_Object[j]->set_r(0.01*(rand() % 100));
							m_Object[j]->set_g(0.01*(rand() % 100));
							m_Object[j]->set_b(0.01*(rand() % 100));
						}
						else if (kind_J == KIND_CARD) {
							m_Object[j]->get_tex(&tx, &ty);

							if (ty != m_attackcard) break;

							m_Object[j]->set_tex(--tx, ty);
							m_Object[j]->set_health(--health_J);
							m_Sound->PlaySound(m_SoundHit, false, 1);
							cout << "health : " << health_J << endl;
						}
					}
					else if (kind_J == KIND_BULLET) {
						m_Object[j]->set_health(--health_J); // bullet

						if (kind_I == KIND_BUILDING) {
							//cout << "Building Check" << endl;
							m_Object[i]->set_r(0.01*(rand() % 100));
							m_Object[i]->set_g(0.01*(rand() % 100));
							m_Object[i]->set_b(0.01*(rand() % 100));
						}
						else if (kind_I == KIND_CARD) {
							m_Object[i]->get_tex(&tx, &ty);

							if (ty != m_attackcard) break;

							m_Object[i]->set_tex(--tx, ty);
							m_Object[i]->set_health(--health_I);
							m_Sound->PlaySound(m_SoundHit, false, 1);
							cout << "health : " << health_I << endl;
						}
					}
				}
			}
		}
	}
}