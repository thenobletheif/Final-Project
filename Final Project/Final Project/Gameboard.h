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

#include "vgl.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Texture.h"
#include "Color.h"
#include "Object.h"
#include "Piece.h"
#include "time.h"

using glm::mat4;
using glm::vec2;
using glm::vec3;
using glm::vec4;

using std::cout;

class Gameboard : public Object
{

public:

	Gameboard();
	~Gameboard();

	void draw(Shader);
	void init();
	void shiftRow(int, bool);
	void shiftColumn(int, bool);



private:

	int moves;
	int points;

	const int numTypes = 6;

	Piece*** board;

	void createRandomPiece(int, int);
	void deletePiece(int, int);
	void checkPairs();
	void fillBoard();
	void pieceFall();

	bool piecesDeleted;
	bool piecesMoved;
	bool emptySpace;
};