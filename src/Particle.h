#pragma once
#include "ofMain.h"

enum particleMode
{
	PARTICLE_MODE_ATTRACT = 0,
	PARTICLE_MODE_REPEL,
	PARTICLE_MODE_NEAREST_POINTS,
	PARTICLE_MODE_NOISE,
	PARTICLE_MODE_PAUSED,
	PARTICLE_MODE_RESUMED,
	PARTICLE_MODE_DRAW,
	PARTICLE_MODE_RESET,
	PARTICLE_MODE_SUP,
	PARTICLE_MODE_SDOWN,
	PARTICLE_MODE_VUP,
	PARTICLE_MODE_VDOWN
};

class Particle
{
private:
	int Pr_color1[3];
	int begin = 0;
	int is_stored = 0;
	int is_paused = 0;
	float stored_pos[2];
	double scale_limit[2];
	int vel_scale = 0;

public:
	Particle();

	void setMode(particleMode newMode);
	void setAttractPoints(vector<glm::vec3> *attract);
	void attractToPoint(int, int);
	void repelFromPoint(int, int);
	void reset();
	void update();
	void draw();

	glm::vec3 pos;
	glm::vec3 vel;
	glm::vec3 frc;
	glm::vec3 stored_vel;
	glm::vec3 stored_frc;

	float drag;
	float uniqueVal;
	float scale;
	int drawpos[2];
	double drawscale;
	int draw_colors[3];

	particleMode mode;

	vector<glm::vec3> *attractPoints;

	//getter

	int get_is_stored() { return is_stored; }
	int get_Prcolor1(int value) { return Pr_color1[value]; }
	int get_is_paused() { return is_paused; }
	int get_stored_pos(int position) { return stored_pos[position]; }
	int get_begin() { return begin; }
	double get_scale_limit(int iterator) { return scale_limit[iterator]; }
	double get_scale() { return scale; }
	int get_vel_scale() { return vel_scale; }
	//setter
	void set_is_stored(int check) { is_stored = check; }
	void set_Prcolor1(int color, int value) { Pr_color1[value] = color; }
	void set_is_paused(int paused_value) { is_paused = paused_value; }
	void set_stored_pos(int position, float value) { stored_pos[position] = value; }
	void set_begin(int start) { begin = start; }
	void set_scale_limit(double scale_value, int i) { scale_limit[i] = scale_value; }
	void set_scale(float scale_val) { scale *= scale_val; }
	void set_vel_scale(int value) { vel_scale = value; }

	// added particles colors in random
	int Particle_Colors()
	{
		if (get_begin() == 0)
		{
			for (int i = 0; i < 3; i++)
			{
				this->set_Prcolor1(ofRandom(255), i);
			}
		}
		this->set_begin(1);

		return 0;
	}
};
