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
#include "Texture.h"

using glm::mat4;
using glm::vec2;
using glm::vec3;
using glm::vec4;

using std::cout;

class Gameboard
{

public:

	Gameboard();
	~Gameboard();

	bool draw(Shader);
	void init(Texture**);
	void shiftRow(bool);
	void shiftColumn(bool);
	void boardClick(float, float);
	void redrawBoard();



private:

	int moves;
	int points;


	static const int BOARD_SIZE = 7;
	const int numTypes = 6;
	const float SPACE_BETWEEN = 0.7;

	Piece* board[BOARD_SIZE][BOARD_SIZE];
	Shader previousShader;

	void createRandomPiece(int, int);
	void deletePiece(int, int);
	void checkPairs();
	void fillBoard();
	void pieceFall();

	bool piecesDeleted;
	bool piecesMoved;
	bool emptySpace;

	void pause(int dur)
	{
		int temp = time(NULL) + dur;

		while (temp > time(NULL));
	}

	glm::vec2 selectedPiece;

	Texture** textures;
};