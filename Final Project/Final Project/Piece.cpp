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

void Piece::rotate(float newAngle, vec3 inAxis)
{
	angles.push_back( newAngle);
	axis.push_back(inAxis);

	Model::rotate(newAngle, inAxis);

}

void Piece::resetAngle()
{
	while (angles.size() > 0)
	{
		Model::rotate(-1 * angles.back(), axis.back());

		angles.pop_back();
		axis.pop_back();
	}
}