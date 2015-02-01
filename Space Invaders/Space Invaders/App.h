#pragma once
#include "MatrixEntities.h"
#include<vector>

#define RANDOM_NUMBER ((float)rand()/(float)RAND_MAX)

class App{
public:
	App();
	~App();

	SDL_Event EVENT;
	SDL_Window *displayWindow;
	const Uint8 *keys;

	void Init();//Sets up windows and entities
	void Update();//Collision detection, movement, and polling input
	void Render();//Draws loaded images to screen
	bool ProcessEvents();//Collects player event input.
	
	float elapsed, lastFrameTicks;

	Entity player;
	Entity backAlien;
	Entity Alien1[10];
	Entity Alien2[10];
	Entity Alien3[10];

	vector<Entity*> Entities;
	GLint playerTexture;
	GLint Alien1texture;
	GLint Alien2texture;
	GLint Alien3texture;


	std::vector<Entity*> Entities;
};