#include "Piece.h"


Piece::Piece()
{
	
}

Piece::~Piece()
{
}

void Piece::init(string filename, int newType)
{
	type = newType;
	Texture* newTexture = new Texture();

	Model::init(filename);

	switch (type)
	{
	case 0:
		newTexture->loadFromFile("Textures/Red.png");
		break;
	case 1:
		newTexture->loadFromFile("Textures/Green.png");
		break;
	case 2:
		newTexture->loadFromFile("Textures/Blue.png");
		break;
	case 3:
		newTexture->loadFromFile("Textures/Yellow.png");
		break;
	case 4:
		newTexture->loadFromFile("Textures/Orange.png");
		break;
	default:
		newTexture->loadFromFile("Textures/Purple.png");
		break;
	}

	Model::setTexture(newTexture);

}

int Piece::getType()
{
	return type;
}