#pragma once

class Object
{
	float posX, posY, posZ;
	float width, height;
	float red, green, blue, alpaha;
	float velX, velY;
	float accX, accY;
	float mass;
	float m_friction = 0.5;
	int rad = 0;
	int kind;

public:
	Object();
	~Object();

	void set(float x, float y, float w, float h, float r, float g, float b, float a);
	void set_x(float x);
	void set_y(float y);
	void set_z(float z);
	void set_vx(float vx);
	void set_vy(float vy);
	void set_ax(float ax);
	void set_ay(float ay);
	void set_w(float w);
	void set_h(float h);
	void set_r(float r); 
	void set_g(float g);
	void set_b(float b); 
	void set_a(float a);
	void set_mass(float m);
	void set_kind(int k);

	void get_pos(float *x, float *y, float *z);
	void get_vel(float *vx, float *vy);
	void get_acc(float *ax, float *ay);
	void get_size(float *w, float *h);
	void get_color(float *r, float *g, float *b, float *a);
	void get_mass(float *m);
	void get_kind(int *k);

	void Update(float eTime);

	void ApplyForce(float fx, float fy, float eTime);
};