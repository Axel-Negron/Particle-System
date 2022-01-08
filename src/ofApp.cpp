#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetVerticalSync(true);

	int num = 1500;
	p.assign(num, Particle());
	currentMode = PARTICLE_MODE_ATTRACT;
	set_stored_state(1);

	currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse";

	resetParticles();

	for (unsigned int i = 0; i < p.size(); i++)
	{

		p[i].set_scale_limit(p[i].get_scale() * (pow(3, 6)), 0);
		p[i].set_scale_limit(p[i].get_scale() * (pow(0.33333333, 5)), 1);
	}
}
//--------------------------------------------------------------
void ofApp::resetParcolor()
{
	for (unsigned int i = 0; i < p.size(); i++)
	{
		p[i].set_begin(0);
	}
}
//--------------------------------------------------------------
void ofApp::resetParticles()
{

	//these are the attraction points used in the fourth demo
	attractPoints.clear();
	for (int i = 0; i < 4; i++)
	{
		attractPoints.push_back(glm::vec3(ofMap(i, 0, 4, 100, ofGetWidth() - 100), ofRandom(100, ofGetHeight() - 100), 0));
	}

	attractPointsWithMovement = attractPoints;

	for (unsigned int i = 0; i < p.size(); i++)
	{
		p[i].setMode(currentMode);
		p[i].setAttractPoints(&attractPointsWithMovement);

		p[i].reset();
	}
}

//--------------------------------------------------------------
void ofApp::update()
{

	//Split update method into two. One works outside DRAW mode, otherr works with the rest of the modes.
	if (currentMode != PARTICLE_MODE_DRAW)
	{
		for (unsigned int i = 0; i < p.size(); i++)
		{
			p[i].setMode(currentMode);
			p[i].update();
		}
	}
	else
	{
		for (unsigned int i = 0; i < p_draw.size(); i++)
		{
			p_draw[i].setMode(currentMode);
			p_draw[i].update();
		}
	}
	//lets add a bit of movement to the attract points
	for (unsigned int i = 0; i < attractPointsWithMovement.size(); i++)
	{
		attractPointsWithMovement[i].x = attractPoints[i].x + ofSignedNoise(i * 10, ofGetElapsedTimef() * 0.7) * 12.0;
		attractPointsWithMovement[i].y = attractPoints[i].y + ofSignedNoise(i * -10, ofGetElapsedTimef() * 0.7) * 12.0;
	}
	//replay function

	if (replay && get_replay_timer() <= stored_counter[get_replay_index() + 1])
	{

		if (stored_mode[get_replay_index()] == PARTICLE_MODE_SUP || stored_mode[get_replay_index()] == PARTICLE_MODE_SDOWN || stored_mode[get_replay_index()] == PARTICLE_MODE_VDOWN || stored_mode[get_replay_index()] == PARTICLE_MODE_VUP || stored_mode[get_replay_index()] == PARTICLE_MODE_RESET)
		{

			if (stored_mode[get_replay_index()] == PARTICLE_MODE_SUP && replay_entered != 1)
			{
				for (unsigned int i = 0; i < p.size(); i++)
				{
					p[i].set_scale(3);
					replay_entered = 1;
				}
			}
			else if (stored_mode[get_replay_index()] == PARTICLE_MODE_SDOWN && replay_entered != 1)
			{
				for (unsigned int i = 0; i < p.size(); i++)
				{
					p[i].set_scale(.333333333);
					replay_entered = 1;
				}
			}
			else if (stored_mode[get_replay_index()] == PARTICLE_MODE_VDOWN && replay_entered != 1)
			{
				for (unsigned int i = 0; i < p.size(); i++)
				{
					p[i].vel /= 4;
					replay_entered = 1;
				}
			}
			else if (stored_mode[get_replay_index()] == PARTICLE_MODE_VUP && replay_entered != 1)
			{
				for (unsigned int i = 0; i < p.size(); i++)
				{
					p[i].vel *= 4;
					replay_entered = 1;
				}
			}
			else if (stored_mode[get_replay_index()] == PARTICLE_MODE_RESET && replay_entered != 1)
			{
				for (unsigned int i = 0; i < p.size(); i++)
				{
					resetParticles();
				}
			}

			set_replay_counter();
		}

		else
		{
			if (stored_mode[get_replay_index()] == PARTICLE_MODE_PAUSED)
			{
				set_begin_timer(0);
			}
			else if (stored_mode[get_replay_index()] == PARTICLE_MODE_RESUMED)
			{
				reset_timer();
				set_begin_timer(1);
			}
			else if (stored_mode[get_replay_index()] == PARTICLE_MODE_NOISE && !replay_entered)
			{
				resetParticles();
				replay_entered = 1;
			}
			currentMode = stored_mode[get_replay_index()];
			set_replay_counter();
			currentModeStr = "Replaying....";
		}
	}
	else if (replay && get_replay_index() < stored_mode.size())
	{
		reset_replay_counter();
		set_replay_index();
		replay_entered = 0;
	}

	else if (replay)
	{
		replay = false;

		currentMode = PARTICLE_MODE_ATTRACT;
		currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse";
		resetParticles();
	}

	start_timer();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	this->color_generator();
	ofBackgroundGradient(ofColor(this->bg_color1[0], this->bg_color1[1], this->bg_color1[2]), ofColor(this->bg_color2[0], this->bg_color2[1], this->bg_color2[2]));

	//Added additional code to implement draw mode.
	if (currentMode != PARTICLE_MODE_DRAW)
	{
		for (unsigned int i = 0; i < p.size(); i++)
		{
			p[i].draw();
		}
		ofSetColor(230);
		string brush_size_text = to_string(get_brush_size());
		ofDrawBitmapString("Draw Controls:\nj-hold to decreases current hue value\nk-hold to increase current hue value\nUse l to cycle between rgb hues.\nm-changes canvas to white", 10, 200);
		ofDrawBitmapString(currentModeStr + "\n\nSpacebar to reset. \nKeys 1-5 to change mode. \nB-Key changes background color. \n[ Pauses simulation.\n] Resumes simulation.\nd-decrease particle size.\ni-increase particle size.\ns-decrease particle velocity.\nf-increase particle velocity.", 10, 20);
	}
	else
	{

		if (p_draw.size() > 0)
		{
			for (unsigned int i = 0; i < p_draw.size(); i++)
			{
				p_draw[i].draw();
			}
		}

		if (!hide_ui)
		{

			string brush_size_text = to_string(get_brush_size());
			preview.draw();
			ofSetColor(0);
			ofDrawBitmapString("Draw Controls:\nj-Decreases current hue value\nk-Increases current hue value\nl-cycle between rgb hues.\nm-Changes canvas to white\nd-Decrease brush size.\ni-Increase brush size.\nh-Hides/Shows UI\n\nBrush size: " + brush_size_text, 10, 100);
			if (get_brush_toggle_cycle() == 0)
			{
				ofSetColor(0);
				ofDrawBitmapString("Changing Red value", 10, 240);
			}
			else if (get_brush_toggle_cycle() == 1)
			{
				ofSetColor(0);
				ofDrawBitmapString("Changing Green value", 10, 240);
			}
			else
			{
				ofSetColor(0);
				ofDrawBitmapString("Changing Blue value", 10, 240);
			}
			ofDrawBitmapString(currentModeStr + "\n\nSpacebar to reset. \nKeys 1-5 to change mode. \nB-Key changes background color.", 10, 20);
		}
	}

	ofSetColor(190);
	if (currentMode == PARTICLE_MODE_NEAREST_POINTS)
	{
		for (unsigned int i = 0; i < attractPoints.size(); i++)
		{
			ofNoFill();
			ofDrawCircle(attractPointsWithMovement[i], 10);
			ofFill();
			ofDrawCircle(attractPointsWithMovement[i], 4);
		}
	}

	//Adds Recording notice in UI, also begins record timer for later use in replay mode.
	if (get_is_recording())
	{
		ofDrawBitmapString("Recording... press r to stop", 10, 300);
		set_record_timer(1);
	}
	if (replay)
	{
		ofDrawBitmapString("Replaying...... press c to cancel", 10, 300);
		ofSetColor(255, 0, 0);
		ofDrawCircle(1000, 670, 20);
	}
}

//------------------------------------------------------------------------------------
void ofApp::start_timer()
//
{
	if (get_begin_timer() == 1)
	{

		if (get_timer() < 5)
		{

			set_timer();
		}

		else
		{

			if (get_stored_state() == 1)
			{
				currentMode = PARTICLE_MODE_ATTRACT;
				currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse";
			}

			else if (get_stored_state() == 2)
			{
				currentMode = PARTICLE_MODE_REPEL;
				currentModeStr = "2 - PARTICLE_MODE_REPEL: repels from mouse";
			}

			else if (get_stored_state() == 3)
			{
				currentMode = PARTICLE_MODE_NEAREST_POINTS;
				currentModeStr = "3 - PARTICLE_MODE_NEAREST_POINTS:";
			}
			else if (get_stored_state() == 4)
			{
				currentMode = PARTICLE_MODE_NOISE;
				currentModeStr = "4 - PARTICLE_MODE_NOISE: snow particle simulation";
			}

			set_begin_timer(0);
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == '1' && !replay)
	{

		if (get_is_recording())
		{

			stored_mode.push_back(PARTICLE_MODE_ATTRACT);
			stored_counter.push_back(get_record_timer());
			reset_record_timer();
		}

		currentMode = PARTICLE_MODE_ATTRACT;
		set_stored_state(1);
		resetParcolor();

		currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse";
	}
	if (key == '2' && !replay)
	{

		if (get_is_recording())
		{

			stored_mode.push_back(PARTICLE_MODE_REPEL);
			stored_counter.push_back(get_record_timer());
			reset_record_timer();
		}

		currentMode = PARTICLE_MODE_REPEL;
		set_stored_state(2);
		resetParcolor();

		currentModeStr = "2 - PARTICLE_MODE_REPEL: repels from mouse";
	}
	if (key == '3' && !replay)
	{

		if (get_is_recording())
		{

			stored_mode.push_back(PARTICLE_MODE_NEAREST_POINTS);
			stored_counter.push_back(get_record_timer());
			reset_record_timer();
		}

		currentMode = PARTICLE_MODE_NEAREST_POINTS;
		set_stored_state(3);
		resetParcolor();

		currentModeStr = "3 - PARTICLE_MODE_NEAREST_POINTS:";
	}
	if (key == '4' && !replay)
	{

		if (get_is_recording())
		{

			stored_mode.push_back(PARTICLE_MODE_NOISE);
			stored_counter.push_back(get_record_timer());
			reset_record_timer();
		}

		currentMode = PARTICLE_MODE_NOISE;
		set_stored_state(4);
		resetParcolor();

		currentModeStr = "4 - PARTICLE_MODE_NOISE: snow particle simulation";
		resetParticles();
	}

	if (key == '5' && !replay)
	{
		reset_brush_size();
		reset_pcounter_val();
		currentMode = PARTICLE_MODE_DRAW;
		currentModeStr = "Drawing:";
		preview.drawscale = 10;
		preview.drawpos[0] = ofGetWindowWidth() - 930;
		preview.drawpos[1] = 15;
		preview.setMode(currentMode);
	}

	if (key == ' ' && !replay)
	{

		if (get_is_recording())
		{

			stored_mode.push_back(PARTICLE_MODE_RESET);
			stored_counter.push_back(get_record_timer());
			reset_record_timer();
		}
		if (currentMode == PARTICLE_MODE_DRAW)
		{
			p_draw.clear();
			reset_pcounter_val();
		}

		resetParticles();
	}

	//Pause key check. Sets mode to paused. Halts particle movement.
	if (key == '[' && !replay)
	{

		if (get_is_recording())
		{

			stored_mode.push_back(PARTICLE_MODE_PAUSED);
			stored_counter.push_back(get_record_timer());
			reset_record_timer();
		}

		currentMode = PARTICLE_MODE_PAUSED;
		currentModeStr = "Paused";
		set_begin_timer(0);
	}

	//Resume key check. Sets mode to stored mode inside RESUMED mode and applies past velocity and friction.
	if (key == ']' && !replay)
	{

		if (get_is_recording())
		{

			stored_mode.push_back(PARTICLE_MODE_RESUMED);
			stored_counter.push_back(get_record_timer());
			reset_record_timer();
		}

		reset_timer();
		set_begin_timer(1);
		currentMode = PARTICLE_MODE_RESUMED;

		currentModeStr = "Resuming...";
	}

	//Changes bbackground color by changing bg_color array in ofApp.
	if (key == 'b' && !replay)
	{

		this->startup = 0;
		this->color_generator();
		ofBackgroundGradient(ofColor(this->bg_color1[0], this->bg_color1[1], this->bg_color1[2]), ofColor(this->bg_color2[0], this->bg_color2[1], this->bg_color2[2]));
	}

	//Increases particle size by changing individual p.size
	if (key == 'i' && !replay)
	{

		if (get_is_recording())
		{

			stored_mode.push_back(PARTICLE_MODE_SUP);
			stored_counter.push_back(get_record_timer());
			reset_record_timer();
		}

		for (unsigned i = 0; i < p.size(); i++)
		{

			if (p[i].get_scale() < p[i].get_scale_limit(0))
			{
				p[i].set_scale((3));
			}
		}
		if (currentMode == PARTICLE_MODE_DRAW && (get_brush_size() + 2) < 1000)
		{
			set_brush_size(2);
		}
	}

	//Decreases particle size by changing individual p.size
	if (key == 'd' && !replay)
	{

		if (get_is_recording())
		{

			stored_mode.push_back(PARTICLE_MODE_SDOWN);
			stored_counter.push_back(get_record_timer());
			reset_record_timer();
		}

		for (unsigned i = 0; i < p.size(); i++)
		{
			if (p[i].get_scale() > p[i].get_scale_limit(1))
			{
				p[i].set_scale((.333333333));
			}
		}
		if (currentMode == PARTICLE_MODE_DRAW && (get_brush_size() - 2) > 0)
		{
			set_brush_size(-2);
		}
	}

	//Starts recording mode, sets up recording vector to store mode, additional vector stores time spent in mode.
	if (key == 'r' && !replay)
	{

		if (get_is_recording())
		{

			set_is_recording(false);
			reset_record_timer();
			resetParticles();
		}

		else
		{
			if (!stored_mode.empty())
			{
				stored_mode.clear();
				stored_counter.clear();
			}
			set_is_recording(true);
		}
	}
	if (key == 'z' && currentMode == PARTICLE_MODE_DRAW && p_draw.size() > 3 && !replay)
	{
		p_draw.pop_back();
		p_draw.pop_back();
		p_draw.pop_back();
		p_draw.pop_back();
		set_pcounter_val(-4);
		if (!p_draw.empty() && get_pcount_draw() < 6)
		{

			p_draw.clear();
			reset_pcounter_val();
		}
	}

	// Keys l,k,j are used to change brush color by passing color val when creating particle at mouse position.
	if (key == 'l' && !replay)
	{
		if (get_brush_toggle_cycle() + 1 <= 2)
		{
			set_brush_color_cycle(1);
		}
		else
		{
			set_brush_color_cycle(-2);
		}
	}

	if (key == 'j' && !replay)
	{
		if (get_brush_color(brush_color_cycle) - 10 >= 0)
		{
			set_brush_color(brush_color_cycle, -10);
			preview.draw_colors[brush_color_cycle] = get_brush_color(brush_color_cycle);
		}
	}
	if (key == 'k' && !replay)
	{
		if (get_brush_color(brush_color_cycle) + 10 >= 0)
		{
			set_brush_color(brush_color_cycle, 10);
			preview.draw_colors[brush_color_cycle] = get_brush_color(brush_color_cycle);
		}
	}

	//Increases p.velocity *=4.
	if (key == 'f' && !replay)
	{
		if (get_is_recording())
		{

			stored_mode.push_back(PARTICLE_MODE_VUP);
			stored_counter.push_back(get_record_timer());
			reset_record_timer();
		}

		for (unsigned int i = 0; i < p.size(); i++)
		{
			p[i].set_vel_scale((p[i].get_vel_scale() + 1));
		};
	}

	//Decreases p.velocity /=4.
	if (key == 's' && !replay)
	{
		if (get_is_recording())
		{

			stored_mode.push_back(PARTICLE_MODE_VDOWN);
			stored_counter.push_back(get_record_timer());
			reset_record_timer();
		}

		for (unsigned int i = 0; i < p.size(); i++)
		{
			p[i].set_vel_scale((p[i].get_vel_scale() - 1));
		};
	}
	if (key == 'p')
	{
		reset_replay_counter();
		reset_replay_index();
		replay = true;
	}
	if (key == 'c' && replay)
	{
		replay = false;
		resetParticles();
	}

	//If in Draw mode checks for m press to change canvas color to white.
	if (key == 'm' && currentMode == PARTICLE_MODE_DRAW)
	{
		set_startup(0);
		for (int i = 0; i < 4; i++)
		{

			set_bg_color1(255, i);
			set_bg_color2(255, i);
		}
		set_startup(1);
	}

	if (key == 'h')
	{
		if (hide_ui)
		{
			hide_ui = 0;
		}
		else
		{
			hide_ui = 1;
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)

//Allows for smooth brush strokes. Creates particles when mouse is dragged after left mouse button is held .
{
	if (held && button == OF_MOUSE_BUTTON_LEFT)
	{
		p_draw.push_back(Particle());
		p_draw[get_pcount_draw()].drawscale = get_brush_size();
		p_draw[get_pcount_draw()].drawpos[0] = ofGetMouseX();
		p_draw[get_pcount_draw()].drawpos[1] = ofGetMouseY();
		p_draw[get_pcount_draw()].draw_colors[0] = get_brush_color(0);
		p_draw[get_pcount_draw()].draw_colors[1] = get_brush_color(1);
		p_draw[get_pcount_draw()].draw_colors[2] = get_brush_color(2);
		set_pcounter_val(1);
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
//Creates particle at mouse pos when pressed.
{
	held = 1;
	if (button == OF_MOUSE_BUTTON_LEFT)
	{

		p_draw.push_back(Particle());
		p_draw[get_pcount_draw()].drawscale = get_brush_size();
		p_draw[get_pcount_draw()].drawpos[0] = ofGetMouseX();
		p_draw[get_pcount_draw()].drawpos[1] = ofGetMouseY();
		p_draw[get_pcount_draw()].draw_colors[0] = get_brush_color(0);
		p_draw[get_pcount_draw()].draw_colors[1] = get_brush_color(1);
		p_draw[get_pcount_draw()].draw_colors[2] = get_brush_color(2);
		set_pcounter_val(1);
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
	held = 0;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
//Centers preview particle when resized.
{
	preview.drawpos[0] = ofGetWindowWidth() - 930;
	preview.drawpos[1] = 15;
	preview.setMode(currentMode);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}
