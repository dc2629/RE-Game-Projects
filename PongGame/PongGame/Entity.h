#pragma once
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_opengl.h>
#include<iostream>
#include<string>
#include<stdio.h>

class Entity{
public:

	Entity();

	float x;
	float y;
	float velocity_x;
	float velocity_y;

	float width;
	float height;
	float rotation;
	float scale_x;
	float scale_y;

	GLint textureID;
	//Additionals
	bool CollidedVertically;

	void Draw();
};