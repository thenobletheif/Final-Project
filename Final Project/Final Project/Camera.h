/*
	Camera.h
	Group: Andrew Joyal, Hunter Ripsom-Gardiner, Connor Williams
	Authors: Andrew Joyal, Hunter Ripsom-Gardiner
	Comments: most code recycled from project 4, updates made by Andrew with help from Hunter
	Date: 10/24/2014
	Course: CSC5210
	Description: Camera
*/

#pragma once

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cstdlib>
#include <math.h>

#include "LoadShaders.h"
#include "vgl.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

#define MAX_MOVE 90
#define PI 3.14159
#define EYE_MOVE_DEFAULT .1	
#define LOOK_MOVE_DEFAULT 2	// in degrees

//using glm::mat4;
using glm::vec3;
using glm::vec4;
using glm::mat4;

class Camera
{
public:
	Camera();
	~Camera();

	void camOut();
	void camIn();
	void camUp();
	void camDown();
	void camLeft();
	void camRight();

	void camLookLeft();
	void camLookRight();
	void camLookUp();
	void camLookDown();

	void setEyeMove(float);
	void setLookAtMove(float);

	void init(float,float,float);
	void reverse();
	void render(Shader);

	float getEyeX();
	float getEyeY();
	float getEyeZ();

	void updateEyeDirection(mat4);
	vec3 getEyeDirection();
	vec3 getEyePosition();
	vec3 getTotalRotation();

	void setFrustumBack(float);
	void updateTarget();

	float getLookX();
	float getLookY();
	float getLookZ();

	mat4 getView();
	void setView(mat4);

private:

	float eye_x;
	float eye_y;
	float eye_z;

	float look_x;
	float look_y;
	float look_z;

	vec3 eyePosition;
	vec3 eyeDirection;

	float target_x;
	float target_y;
	float target_z;

	float eye_move;
	float look_move;

	vec3 totalRotation;

	mat4 view;
	mat4 eyeRotate;
	mat4 position;
	mat4 frustum;
	mat4 rotateMatrix;
};

