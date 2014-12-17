#include "Gameboard.h"


Gameboard::Gameboard()
{

}

Gameboard::~Gameboard()
{
}

bool Gameboard::draw(Shader shaderFile)
{
	bool doodoo = false;

	previousShader = shaderFile;

	if (piecesMoved)
	{
		checkPairs();
		piecesMoved = false;
		doodoo = true;
	}
	else if (piecesDeleted)
	{
		pieceFall();
		piecesDeleted = false;
		doodoo = true;
	}
	else if (emptySpace)
	{
		fillBoard();
		emptySpace = false;
		doodoo = true;
	}

	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++)
		{
			if (board[i][j] != NULL)
				board[i][j]->draw(shaderFile);
		}

	return doodoo;
}

void Gameboard::redrawBoard()
{
	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++)
		{
			if (board[i][j] != NULL)
				board[i][j]->draw(previousShader);
		}
	//glFlush;
	//pause(2);
}

void Gameboard::init(Texture** newTextures)
{
	srand(time(NULL));

	emptySpace = false;
	piecesDeleted = false;
	piecesMoved = false;

	moves = 0;
	points = 0;

	textures = newTextures;

	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			createRandomPiece(i, j);
		}
	}

	selectedPiece[0] = int(BOARD_SIZE/2);
	selectedPiece[1] = int(BOARD_SIZE/2);


}

void boardClick(float x, float y)
{


}

void Gameboard::shiftRow(bool right)
{
	int row = selectedPiece[1];

	if (right)
	{
		deletePiece(row, 6);

		for (int i = 6; i > 0; i--)
		{
			board[row][i] = board[row][i - 1];
			board[row][i]->translate(SPACE_BETWEEN, 0, 0);
		}

		createRandomPiece(row, 0);
	}
	else
	{
		deletePiece(row, 0);

		for (int i = 0; i < 6; i++)
		{
			board[row][i] = board[row][i + 1];
			board[row][i]->translate(-1 * SPACE_BETWEEN, 0, 0);
		}

		createRandomPiece(row, 6);
	}

	piecesMoved = true;

	redrawBoard();

}

void Gameboard::shiftColumn(bool up)
{
	int column = selectedPiece[0];

	if (up)
	{
		deletePiece(6, column);

		for (int i = 6; i > 0; i--)
		{
			board[column][i] = board[column][i - 1];
			board[column][i]->translate(0, SPACE_BETWEEN, 0);
		}

		createRandomPiece(column, 0);
	}
	else
	{
		deletePiece(column, 0);

		for (int i = 0; i < 6; i++)
		{
			board[column][i] = board[column][i + 1];
			board[column][i]->translate(0, -1 * SPACE_BETWEEN, 0);
		}

		createRandomPiece(column, 6);
	}

	piecesMoved = true;

	redrawBoard();
	pause(1);

}

void Gameboard::deletePiece(int row, int column)
{
	//delete(board[row][column]);
	board[row][column] = NULL;

}

void Gameboard::createRandomPiece(int row, int column)
{

	board[row][column] = new Piece();

	switch (rand() % numTypes)
	{
	case 0:
		board[row][column]->init("Models/mineCraftCube.obj", 0, textures[0]);
		break;
	case 1:
		board[row][column]->init("Models/mineCraftCube.obj", 1, textures[1]);
		break;
	case 2:
		board[row][column]->init("Models/mineCraftCube.obj", 2, textures[2]);
		break;
	case 3:
		board[row][column]->init("Models/mineCraftCube.obj", 3, textures[3]);
		break;
	case 4:
		board[row][column]->init("Models/mineCraftCube.obj", 4, textures[4]);
		break;
	default:
		board[row][column]->init("Models/mineCraftCube.obj", 5, textures[5]);
		break;
	}

	board[row][column]->translate((((-1.0 * SPACE_BETWEEN * (BOARD_SIZE - 1.0)) / 2.0) + SPACE_BETWEEN*row) - (board[row][column]->center[0]), (((-1.0 * SPACE_BETWEEN * (BOARD_SIZE - 1.0)) / 2.0) + SPACE_BETWEEN*column) - (board[row][column]->center[1]), 0);

	board[row][column]->scale(0.15);
}

void Gameboard::checkPairs()
{
	bool pairedPieces[7][7] =
	{ false, false, false, false, false, false, false,
	false, false, false, false, false, false, false,
	false, false, false, false, false, false, false,
	false, false, false, false, false, false, false,
	false, false, false, false, false, false, false,
	false, false, false, false, false, false, false,
	false, false, false, false, false, false, false };

	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (board[i][j] != NULL && board[i][j + 1] != NULL)
			if (board[i][j]->getType() == board[i][j + 1]->getType())
			{
				pairedPieces[i][j] = true;
				pairedPieces[i][j + 1] = true;
				piecesDeleted = true;
			}
		}
	}

	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (board[j][i] != NULL && board[j + 1][i] != NULL)
			if (board[j][i]->getType() == board[j + 1][i]->getType())
			{
				pairedPieces[j][i] = true;
				pairedPieces[j + 1][i] = true;
				piecesDeleted = true;
			}
		}
	}

	//If any pair was found, go through and delete all pairs found
	if (piecesDeleted)
		for (int i = 0; i < 7; i++)
			for (int j = 0; j < 7; j++)
				if (pairedPieces[i][j])
					deletePiece(i, j);
}

void Gameboard::pieceFall()
{
	int start;

	for (int i = 0; i < 7; i++)
	{
		start = 8;

		for (int j = 0; j < 7; j++)
		{
			if (board[i][j] == NULL)
			{
				start = j;
				j = 7;
			}
		}

		for (int j = start + 1; j < 7; j++)
		{
			if (board[i][j] != NULL)
			{
				board[i][start] = board[i][j];
				board[i][j] = NULL;
				board[i][start]->translate(0, -1 * SPACE_BETWEEN * (j - start), 0);
				start++;
				piecesMoved = true;
			}
		}
	}

	emptySpace = true;
}

void Gameboard::fillBoard()
{
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (board[i][j] == NULL)
			{
				createRandomPiece(i, j);
			}
		}
	}
	piecesMoved = true;
}