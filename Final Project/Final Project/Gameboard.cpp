#include "Gameboard.h"


Gameboard::Gameboard()
{

}

Gameboard::~Gameboard()
{
}

void Gameboard::reDraw(Shader shaderFile)
{
	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++)
		{
			if (board[i][j] != NULL)
				board[i][j]->draw(shaderFile);
		}
}

bool Gameboard::draw(Shader shaderFile)
{
	bool pauseRequest = false;

	previousShader = shaderFile;
	
	if (piecesShifted)
	{
		cout << "shifted\n";
		piecesShifted = false;

		pauseRequest = true;
	}
	else if (piecesMoved)
	{
		cout << "delete\n";
		checkPairs();
		piecesMoved = false;

		pauseRequest = true;
	}
	else if (piecesDeleted)
	{
		cout << "fall\n";
		pieceFall();
		piecesDeleted = false;

		pauseRequest = true;
	}
	else if (emptySpace)
	{
		cout << "fill\n";
		fillBoard();
		emptySpace = false;

		pauseRequest = true;
	}

	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++)
		{
			if (board[i][j] != NULL)
				board[i][j]->draw(shaderFile);
		}

	return pauseRequest;
}

void Gameboard::init(Texture** newTextures)
{
	srand(time(NULL));

	lastMousePos[0] = 0;
	lastMousePos[1] = 0;

	highlightedPiece[0] = -1;
	highlightedPiece[1] = -1;

	piecesShifted = false;
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

	int tempX = selectedPiece[0];
	int tempY = selectedPiece[1];
	board[tempX][tempY]->scale(1.25);


}

void Gameboard::unSelectPiece()
{
	int tempX = selectedPiece[0];
	int tempY = selectedPiece[1];

	if (board[tempX][tempY] != NULL)
		board[tempX][tempY]->scale(0.8);
}

void Gameboard::reSelectPiece()
{
	int tempX = selectedPiece[0];
	int tempY = selectedPiece[1];

	if (board[tempX][tempY] != NULL)
		board[tempX][tempY]->scale(1.25);
}

void Gameboard::selectPiece(int x, int y)
{
	int tempX = selectedPiece[0];
	int tempY = selectedPiece[1];
	board[tempX][tempY]->scale(0.8);

	selectedPiece[0] = int((x - 100)/(710 / BOARD_SIZE));
	selectedPiece[1] = BOARD_SIZE - 1 - int(y / (685 / BOARD_SIZE));	//because screen coordinate y increases as it goes down, flip it

	tempX = selectedPiece[0];
	tempY = selectedPiece[1];
	board[tempX][tempY]->scale(1.25);
}

void Gameboard::shiftRow(bool up)
{
	unSelectPiece();

	moves++;
	int row = selectedPiece[1];

	if (up)
	{
		//deletePiece(row, 6);

		for (int i = 6; i > 0; i--)
		{
			*board[i][row] = *board[i - 1][row];
			board[i][row]->translate(SPACE_BETWEEN, 0, 0);
		}

		createRandomPiece(0, row);
	}
	else
	{
		//deletePiece(row, 0);

		for (int i = 0; i < 6; i++)
		{
			*board[i][row] = *board[i + 1][row];
			board[i][row]->translate(-1 * SPACE_BETWEEN, 0, 0);
		}

		createRandomPiece(6, row);
	}

	piecesShifted = true;
	piecesMoved = true;

	reSelectPiece();

}

void Gameboard::shiftColumn(bool right)
{
	unSelectPiece();

	moves++;
	int column = selectedPiece[0];

	if (right)
	{
		//deletePiece(6, column);
		
		for (int i = 6; i > 0; i--)
		{
			*board[column][i] = *board[column][i - 1];
			board[column][i]->translate(0, SPACE_BETWEEN, 0);
		}

		createRandomPiece(column, 0);
	}
	else
	{
		//deletePiece(column, 0);

		for (int i = 0; i < 6; i++)
		{
			*board[column][i] = *board[column][i + 1];
			board[column][i]->translate(0, -1 * SPACE_BETWEEN, 0);
		}

		createRandomPiece(column, 6);
	}

	piecesShifted = true;
	piecesMoved = true;

	reSelectPiece();
}

void Gameboard::deletePiece(int row, int column)
{
	delete(board[row][column]);
	board[row][column] = NULL;

}

void Gameboard::createRandomPiece(int row, int column)
{

	board[row][column] = new Piece();

	int randomType = rand() % numTypes;
	board[row][column]->init("Models/mineCraftCube.obj", randomType, textures[randomType]);


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
	unSelectPiece();

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

	reSelectPiece();
}

void Gameboard::highlightPiece()
{
	int tempX = highlightedPiece[0];
	int tempY = highlightedPiece[1];



	tempX = int((lastMousePos[0] - 100) / (710 / BOARD_SIZE));
	tempY = BOARD_SIZE - 1 - int(lastMousePos[1] / (685 / BOARD_SIZE));

	if (highlightedPiece[0] < BOARD_SIZE && highlightedPiece[0] >= 0 && highlightedPiece[1] < BOARD_SIZE && highlightedPiece[1] >= 0)
	{
		if (tempX != highlightedPiece[0] || tempY != highlightedPiece[1])
			board[tempX][tempY]->resetAngle();
	}


	highlightedPiece[0] = tempX;
	highlightedPiece[1] = tempY;

	if (highlightedPiece[0] < BOARD_SIZE && highlightedPiece[0] >= 0 && highlightedPiece[1] < BOARD_SIZE && highlightedPiece[1] >= 0)
	{
		board[tempX][tempY]->rotate(0.005, { 0, 1, 0 });
	}

}

void Gameboard::highlightPiece(int x, int y)
{
	lastMousePos[0] = x;
	lastMousePos[1] = y;

	
	int tempX = highlightedPiece[0];
	int tempY = highlightedPiece[1];

	

	tempX = int((x - 100) / (710 / BOARD_SIZE));
	tempY = BOARD_SIZE - 1 - int(y / (685 / BOARD_SIZE));

	if (highlightedPiece[0] < BOARD_SIZE && highlightedPiece[0] >= 0 && highlightedPiece[1] < BOARD_SIZE && highlightedPiece[1] >= 0)
	{
		if (tempX != highlightedPiece[0] || tempY != highlightedPiece[1])
			board[tempX][tempY]->resetAngle();
	}


	highlightedPiece[0] = tempX;
	highlightedPiece[1] = tempY;
	
	if (highlightedPiece[0] < BOARD_SIZE && highlightedPiece[0] >= 0 && highlightedPiece[1] < BOARD_SIZE && highlightedPiece[1] >= 0)
	{
		board[tempX][tempY] -> rotate(1, { 0, 1, 0 });
	}
	
}


void Gameboard::fillBoard()
{
	unSelectPiece();

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

	reSelectPiece();
}