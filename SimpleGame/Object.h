#pragma once

class Object
{
	float m_posX, m_posY, m_posZ;
	float m_width, m_height;
	float red, green, blue, alpaha;
	float m_velX, m_velY, m_velZ;
	float m_accX, m_accY, m_accZ;
	float m_mass;
	float m_friction = 0.5;
	int m_rad = 0;
	int m_kind;
	int m_health = 1;
	int m_state;


public:
	Object();
	~Object();

	void set(float x, float y, float w, float h, float r, float g, float b, float a);
	void set_x(float x);
	void set_y(float y);
	void set_z(float z);
	void set_vx(float vx);
	void set_vy(float vy);
	void set_vz(float vz);

	void set_ax(float ax);
	void set_ay(float ay);
	void set_az(float az);

	void set_w(float w);
	void set_h(float h);
	void set_r(float r); 
	void set_g(float g);
	void set_b(float b); 
	void set_a(float a);
	void set_mass(float m);
	void set_kind(int k);
	void set_health(int h);
	void set_state(int s);

	void get_pos(float *x, float *y, float *z);
	void get_vel(float *vx, float *vy, float *vz);
	void get_acc(float *ax, float *ay, float *az);
	void get_size(float *w, float *h);
	void get_color(float *r, float *g, float *b, float *a);
	void get_mass(float *m);
	void get_kind(int *k);
	void get_health(int *h);
	void get_state(int *s);

	void Update(float eTime);

	void ApplyForce(float fx, float fy, float fz, float eTime);
};