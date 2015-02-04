#pragma once
#include "MatrixEntities.h"
#include<vector>

#define RANDOM_NUMBER ((float)rand()/(float)RAND_MAX)
#define FIXED_TIMESTEP 0.016667
#define MAX_TIMESTEP 6

class App{
public:
	App();
	~App();

	SDL_Event EVENT;
	SDL_Window *displayWindow;
	const Uint8 *keys;

	void Init();//Sets up windows and entities
	void Update();//Animation and non-collision updates to entity
	void FixedUpdate();//Collision Code
	void UpdateandRender();
	void Render();//Draws loaded images to screen
	bool ProcessEvents();//Collects player event input.
	void GameOverRender();
	void GameOverUpdate();
	
	float elapsed, lastFrameTicks,timeLeftOver, actualElapsed, delay;
	//Entities
	Entity player;
	Entity backAlien;
	Entity Alien1[11];
	Entity Alien2r1[11];
	Entity Alien2r2[11];
	Entity Alien3r1[11];
	Entity Alien3r2[11];
	Entity background;

	GLint playerTexture;
	GLint Alien1texture;
	GLint Alien2texture;
	GLint Alien3texture;
	GLint font;
	
	std::vector<Entity*> Entities;

	//Movement and Animation
	bool AlienMovement;
	float AlienMvtSpeedModifier;
	float AlienAnimationTimer;
	bool ShiftAliensDown;
	bool RightorLeftSide;
	bool backALienMvt;

	//Interface
	Entity playerlives[3];
	int Score;

	//Bullets
	Entity playerbullet;//I don't know why but apparently there's only 1 bullet in space invader that can only be shot once it's out of the screen or invisible.
	Entity AIbullets[7];
	int randomAIfinder();
	int health;
};