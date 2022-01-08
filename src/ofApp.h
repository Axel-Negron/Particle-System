#pragma once

#include "ofMain.h"
#include "Particle.h"

class ofApp : public ofBaseApp
{

	//Added store_bg value ofApp, Added various vars that are used in different modes.
private:
	int bg_color1[3], bg_color2[3];
	int startup = 0;
	int begin_timer = 0;
	int timer;
	int stored_state;
	bool is_recording = false;
	int record_timer = 0;

	int p_counterdraw;
	int brush_size;
	int brush_colors[3];
	int brush_color_cycle = 0;
	int replay_timer = 0;
	unsigned int replay_index = 0;

public:
	void setup();
	void update();
	void draw();
	void resetParticles();
	void start_timer();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void resetParcolor();

	particleMode currentMode;
	string currentModeStr;
	bool held;
	bool replay_entered;
	bool cycle_back = 0;
	bool hide_ui = 0;

	vector<Particle> p;
	vector<Particle> p_draw;
	vector<particleMode> stored_mode;
	vector<int> stored_counter;
	vector<glm::vec3> attractPoints;
	vector<glm::vec3> attractPointsWithMovement;
	Particle preview = Particle();

	bool replay;

	//getter_fortests
	int get_startup() { return startup; }
	int get_timer() { return timer; }
	int get_begin_timer() { return begin_timer; }
	int get_stored_state() { return stored_state; }
	bool get_is_recording() { return is_recording; }
	int get_record_timer() { return record_timer; }

	int get_pcount_draw() { return p_counterdraw; }
	int get_brush_size() { return brush_size; }
	int get_brush_color(int pos) { return brush_colors[pos]; }
	int get_brush_toggle_cycle() { return brush_color_cycle; }
	int get_replay_index() { return replay_index; }
	int get_replay_timer() { return replay_timer; }

	//setter for bg_color
	void set_bg_color1(int color, int i) { bg_color1[i] = color; }
	void set_bg_color2(int color, int i) { bg_color2[i] = color; }
	void reset_timer() { timer = 0; }
	void set_begin_timer(int value) { begin_timer = value; }
	void set_timer() { timer += 1; }
	void set_startup(int x) { startup = x; }
	void set_stored_state(int state_val) { stored_state = state_val; }
	void set_is_recording(bool value) { is_recording = value; }
	void set_record_timer(int increment) { record_timer += increment; }
	void reset_record_timer() { record_timer = 0; }

	void reset_pcounter_val() { p_counterdraw = 0; }
	void set_pcounter_val(int val) { p_counterdraw += val; }
	void reset_brush_size() { brush_size = 15; }
	void set_brush_size(int val) { brush_size += val; }
	void set_brush_color(int pos, int hue) { brush_colors[pos] += hue; }
	void set_brush_color_cycle(int val) { brush_color_cycle += val; }
	void set_replay_index() { replay_index++; }
	void set_replay_counter() { replay_timer++; }
	void reset_replay_index() { replay_index = 0; }
	void reset_replay_counter() { replay_timer = 0; }
	//Adding Random_color_generator_function

	int color_generator()
	{
		if (get_startup() == 0)
		{
			for (int i = 0; i < 3; i++)
			{
				this->set_bg_color1(floor(ofRandom(255)), i);

				this->set_bg_color2(floor(ofRandom(255)), i);
			}
			this->set_startup(1);
		}
		return 0;
	}
};
