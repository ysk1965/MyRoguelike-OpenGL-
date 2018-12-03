#include "stdafx.h"
#include "Object.h"
#include <math.h>
#include <float.h>
#include <iostream>

Object::Object()
{	
}


Object::~Object()
{
}

void Object::set(float x, float y, float w, float h, float r, float g, float b, float a)
{
	m_posX = x; m_posY = y;
	m_width = w; m_height = h;
	red = r; green = g; blue = b; alpaha = a;
}
void Object::set_x(float x) { m_posX = x; }
void Object::set_y(float y) { m_posY = y; }
void Object::set_z(float z) { m_posZ = z; }
void Object::set_vx(float vx) { m_velX = vx; }
void Object::set_vy(float vy) { m_velY = vy; }
void Object::set_vz(float vz) { m_velZ = vz; }
void Object::set_ax(float ax) { m_accX = ax; }
void Object::set_ay(float ay) { m_accY = ay; }
void Object::set_az(float az) { m_accZ = az; }
void Object::set_w(float w) { m_width = w; }
void Object::set_h(float h) { m_height = h; }
void Object::set_r(float r) { red = r; }
void Object::set_g(float g) { green = g; }
void Object::set_b(float b) { blue = b; }
void Object::set_a(float a) { alpaha = a; }
void Object::set_mass(float m) { m_mass = m; }
void Object::set_kind(int k) { m_kind = k; }
void Object::set_health(int h) { m_health = h; }
void Object::set_state(int s) { m_state = s; }

void Object::get_pos(float *x, float *y, float *z) { *x = m_posX; *y = m_posY; *z = m_posZ; }
void Object::get_vel(float *vx, float *vy, float *vz) { *vx = m_velX; *vy = m_velY; *vz = m_velZ; }
void Object::get_acc(float *ax, float *ay, float *az) { *ax = m_accX; *ay = m_accY; *az = m_accZ; }
void Object::get_size(float *w, float *h) { *w = m_width; *h = m_height; }
void Object::get_color(float *r, float *g, float *b, float *a) { *r = red; *g = green; *b = blue; *a = alpaha; }
void Object::get_mass(float *m) { *m = m_mass; }
void Object::get_kind(int *k) { *k = m_kind; }
void Object::get_health(int *h) { *h = m_health; }
void Object::get_state(int * s) { *s = m_state; }

void Object::Update(float eTime)
{
	float velocity = sqrtf(m_velX*m_velX + m_velY * m_velY + m_velZ * m_velZ); //size ofvelocity

	if (velocity < FLT_EPSILON);
	else
	{
		//gravity force
		float gz;
		gz = 9.8 * m_mass; // == 수직항력
		float friction;
		friction = m_friction * gz;
		
		//방향
		float fx, fy;
		fx = -m_velX * friction / velocity;
		fy = -m_velY * friction / velocity;

		//Clac acc
		float ax, ay;
		ax = fx / m_mass;
		ay = fy / m_mass;

		float afterVelX = m_velX + eTime * ax;
		float afterVelY = m_velY + eTime * ay;
		
		if (m_velX * afterVelX < 0)
			m_velX = 0;
		else 
			m_velX = afterVelX;
		if (m_velY * afterVelY < 0)
			m_velY = 0;
		else
			m_velY = afterVelY;
	}

	m_velX = m_velX + eTime * m_accX;
	m_velY = m_velY + eTime * m_accY;
	m_velZ = m_velZ + eTime * m_accZ;

	m_posX = m_posX + eTime * m_velX;
	m_posY = m_posY + eTime * m_velY;
	m_posZ = m_posZ + eTime * m_velZ;

	m_posZ = m_posZ + (sin(m_rad * 3.14 / 180)) * 0.05;
	m_rad = (m_rad + 10) % 360;

}

void Object::ApplyForce(float fx, float fy, float fz, float eTime)
{
	m_accX = fx / m_mass;
	m_accY = fy / m_mass;
	m_accZ = fz / m_mass;

	m_velX = m_velX + eTime * m_accX;
	m_velY = m_velY + eTime * m_accY;
	m_velZ = m_velZ + eTime * m_accZ;
	m_accY = 0;

	m_accX = 0.f;
	m_accY = 0.f;
	m_accZ = 0.f;
}