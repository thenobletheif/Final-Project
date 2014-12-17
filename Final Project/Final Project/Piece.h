/*
Model.h
Group: Andrew Joyal, Hunter Ripsom-Gardiner, Connor Williams
Authors: Andrew Joyal
Comments: most code recycled from project 4
Date: 10/24/2014
Course: CSC5210
Description: Model
*/

#pragma once

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

#include <vector>
#include "vgl.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Texture.h"
#include "Color.h"
#include "Model.h"

using glm::mat4;
using glm::vec2;
using glm::vec3;
using glm::vec4;

using std::cout;

class Piece : public Model
{

public:

	Piece();
	~Piece();

	void init(string, int, Texture*);
	int getType();
	void resetAngle();

	void rotate(float, vec3);

	


	

private:

	vector <float> angles;
	vector <vec3> axis;

	int type;
	
};
