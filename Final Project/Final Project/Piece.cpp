#include "Piece.h"


Piece::Piece()
{
	
}

Piece::~Piece()
{
}

void Piece::init(string filename, int newType, Texture* newTexture)
{
	type = newType;

	Model::init(filename);

	Model::setTexture(newTexture);

}

int Piece::getType()
{
	return type;
}