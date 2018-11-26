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
	posX = x; posY = y;
	width = w; height = h;
	red = r; green = g; blue = b; alpaha = a;
}
void Object::set_x(float x) { posX = x; }
void Object::set_y(float y) { posY = y; }
void Object::set_z(float z) { posZ = z; }
void Object::set_vx(float vx) { velX = vx; }
void Object::set_vy(float vy) { velY = vy; }
void Object::set_ax(float ax) { accX = ax; }
void Object::set_ay(float ay) { accY = ay; }
void Object::set_w(float w) { width = w; }
void Object::set_h(float h) { height = h; }
void Object::set_r(float r) { red = r; }
void Object::set_g(float g) { green = g; }
void Object::set_b(float b) { blue = b; }
void Object::set_a(float a) { alpaha = a; }
void Object::set_mass(float m) { mass = m; }
void Object::set_kind(int k) { kind = k; }

void Object::get_pos(float *x, float *y, float *z) { *x = posX; *y = posY; *z = posZ; }
void Object::get_vel(float *vx, float *vy) { *vx = velX; *vy = velY; }
void Object::get_acc(float *ax, float *ay) { *ax = accX; *ay = accY; }
void Object::get_size(float *w, float *h) { *w = width; *h = height; }
void Object::get_color(float *r, float *g, float *b, float *a) { *r = red; *g = green; *b = blue; *a = alpaha; }
void Object::get_mass(float *m) { *m = mass; }
void Object::get_kind(int *k) { *k = kind; }

void Object::Update(float eTime)
{
	float velocity = sqrtf(velX*velX + velY * velY); //size ofvelocity

	if (velocity < FLT_EPSILON);
	else
	{
		//gravity force
		float gz;
		gz = 9.8 * mass; // == 수직항력
		float friction;
		friction = m_friction * gz;
		
		//방향
		float fx, fy;
		fx = -velX * friction / velocity;
		fy = -velY * friction / velocity ;

		//Clac acc
		float ax, ay;
		ax = fx / mass;
		ay = fy / mass;

		float afterVelX = velX + eTime * ax;
		float afterVelY = velY + eTime * ay;
		
		if (velX * afterVelX < 0)
			velX = 0;
		else 
			velX = afterVelX;
		if (velY * afterVelY < 0)
			velY = 0;
		else
			velY = afterVelY;
	}

	velX = velX + eTime * accX;
	velY = velY + eTime * accY;

	posX = posX + eTime * velX;
	posY = posY + eTime * velY;

	posZ = posZ + (sin(rad * 3.14 / 180)) * 0.05;
	rad = (rad + 10) % 360;

}

void Object::ApplyForce(float fx, float fy, float eTime)
{
	accX = fx / mass;
	accY = fy / mass;

	velX = velX + eTime * accX;
	velY = velY + eTime * accY;
		accY = 0;

	accX = 0.f;
	accY = 0.f;
}