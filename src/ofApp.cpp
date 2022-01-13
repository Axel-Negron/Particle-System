#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetVerticalSync(true);
	gui.setup("","settings.xml",ofGetWidth()-250,10);
	gui.add(colors.setup("Change","color",20,10));
	gui.add(R.setup("R",0,0,255));
	gui.add(G.setup("G",0,0,255));
	gui.add(B.setup("B",0,0,255));	
	gui.add(sizelbl.setup("Change","size",20,10));
	gui.add(size.setup("Brush size",15,1,100));
	gui.add(clearcanvas.setup("Clear","Canvas",20,10));
	gui.add(cleartoggle.setup("Clear Canvas"));
	
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
		preview.draw_colors[0] = R; preview.draw_colors[1] = G; preview.draw_colors[2] = B;
		for (unsigned int i = 0; i < p_draw.size(); i++)
		{
			p_draw[i].setMode(currentMode);
			p_draw[i].update();
		}
		if(cleartoggle){
			p_draw.clear();
			reset_pcounter_val();
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
		string brush_size_text = to_string(size);
		ofDrawBitmapString("Draw Controls:\nm-changes canvas to white", 10, 200);
		ofDrawBitmapString(currentModeStr + "\n\nSpacebar to reset. \nKeys 1-5 to change mode. \nB-Key changes background color. \n[ Pauses simulation.\n] Resumes simulation.\ns-decrease particle velocity.\nf-increase particle velocity.", 10, 20);
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

			string brush_size_text = to_string(size);
			preview.draw();
			ofSetColor(0);
			ofDrawBitmapString("Draw Controls:\nm-Changes canvas to white\nd-Decrease brush size.\ni-Increase brush size.\nh-Hides/Shows UI\n\nBrush size: " + brush_size_text, 10, 100);
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
	gui.draw();
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
		size = 15;
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


	//Decreases particle size by changing individual p.size


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
		p_draw[get_pcount_draw()].drawscale = size;
		p_draw[get_pcount_draw()].drawpos[0] = ofGetMouseX();
		p_draw[get_pcount_draw()].drawpos[1] = ofGetMouseY();
		p_draw[get_pcount_draw()].draw_colors[0] = R;
		p_draw[get_pcount_draw()].draw_colors[1] = G;
		p_draw[get_pcount_draw()].draw_colors[2] = B;
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
		p_draw[get_pcount_draw()].drawscale = size;
		p_draw[get_pcount_draw()].drawpos[0] = ofGetMouseX();
		p_draw[get_pcount_draw()].drawpos[1] = ofGetMouseY();
		p_draw[get_pcount_draw()].draw_colors[0] = R;
		p_draw[get_pcount_draw()].draw_colors[1] = G;
		p_draw[get_pcount_draw()].draw_colors[2] = B;
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
