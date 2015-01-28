#pragma once
#include "Entity.h"
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

	Entity paddle1, paddle2;
	Entity ball;
	Entity background;

	std::vector<Entity*> Entities;
};