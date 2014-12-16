#include "Gameboard.h"


Gameboard::Gameboard()
{

}

Gameboard::~Gameboard()
{
}

void Gameboard::draw(Shader shaderFile)
{
	if (piecesMoved)
	{
		checkPairs();
		piecesMoved = false;
	}
	else if (piecesDeleted)
	{
		pieceFall();
		piecesDeleted = false;
	}
	else if (emptySpace)
	{
		fillBoard();
		emptySpace = false;
	}

	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++)
		{
			if (board[i][j] != NULL)
				board[i][j]->draw(shaderFile);
		}

}


void Gameboard::init(Texture** newTextures)
{
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

	selectedPiece = board[int(BOARD_SIZE / 2)][(BOARD_SIZE/2)];


}

void boardClick(float x, float y)
{


}

void Gameboard::shiftRow(bool right)
{
	int row = selectedPiece->center[0];

	if (right)
	{
		deletePiece(6, row);

		for (int i = 6; i > 0; i--)
		{
			board[i][row] = board[i - 1][row];
			board[i][row]->translate((-0.7 + 0.2*row) - (board[i][row]->center[0]), 0, 0);
		}

		createRandomPiece(0, row);
	}
	else
	{
		deletePiece(0, row);

		for (int i = 0; i < 7; i++)
		{
			board[i][row] = board[i + 1][row];
			board[i][row]->translate((-0.7 + 0.2*row) - (board[i][row]->center[0]), 0, 0);
		}

		createRandomPiece(6, row);
	}

	piecesMoved = true;

}

void Gameboard::shiftColumn(bool up)
{
	int column = selectedPiece->center[1];

	if (up)
	{
		deletePiece(6, column);

		for (int i = 6; i > 0; i--)
		{
			board[i][column] = board[i - 1][column];
			board[i][column]->translate((-0.7 + 0.2*column) - (board[i][column]->center[0]), 0, 0);
		}

		createRandomPiece(0, column);
	}
	else
	{
		deletePiece(0, column);

		for (int i = 0; i < 7; i++)
		{
			board[i][column] = board[i + 1][column];
			board[i][column]->translate((-0.7 + 0.2*column) - (board[i][column]->center[0]), 0, 0);
		}

		createRandomPiece(6, column);
	}

	piecesMoved = true;

}

void Gameboard::deletePiece(int row, int column)
{
	delete(board[row][column]);
	board[row][column] = NULL;

}

void Gameboard::createRandomPiece(int row, int column)
{
	srand(time(NULL));

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
			{
				if (pairedPieces[i][j])
					deletePiece(i, j);
			}
}

void Gameboard::pieceFall()
{
	int start;

	for (int i = 0; i < 7; i++)
	{
		start = 8;

		for (int j = 6; j >= 0; j--)
		{
			if (board[i][j] == NULL)
			{
				start = j;
				j = -1;
			}
		}

		for (int j = start + 1; j < 7; j++)
		{
			if (board[i][j] != NULL)
			{
				board[i][start] = board[i][j];
				board[i][j] = NULL;
				board[i][start]->translate(0, (-0.7 + 0.2*start) - (board[i][start]->center[1]), 0);
				start++;
				piecesMoved = true;
				emptySpace = true;
			}
		}
	}
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
}