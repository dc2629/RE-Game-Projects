#pragma once
#include"Color.h"
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_opengl.h>
#include<stdio.h>
#include<vector>
#include<algorithm>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include<SDL_mixer.h>

#define RANDOM_NUMBER ((float)rand()/(float)RAND_MAX)
#define PI 3.14159265359

using namespace std;

class Vector {
public:
	Vector();
	Vector(float x, float y, float z);
	float length();
	void normalize();
	float x;
	float y;
	float z;
	void dispV();
};

class Matrix {
public:
	Matrix();
	union {
		float m[4][4];
		float ml[16];
	};
	void identity();
	Matrix inverse();
	Matrix operator*(const Matrix &m2);
	Vector operator*(const Vector &v);
	void dispM();
};

class Entity {
public:
	Entity();
	Matrix matrix;
	float x;
	float y;

	float width;
	float height;

	float scale_x;
	float scale_y;

	float rotation;

	bool visible;

	float velocity_x;
	float velocity_y;

	float acceleration_x;
	float acceleration_y;

	void buildMatrix();
	void Render();

	GLint textureID;
	int index;
	int spriteCountX;
	int spriteCountY;

	void Draw();

	void resetCollisions();
	bool checkCollision(Entity A);
};

