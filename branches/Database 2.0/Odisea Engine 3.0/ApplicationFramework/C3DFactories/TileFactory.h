#ifndef TILEFACTORY
#define TILEFACTORY

#include "Factory.h"

class TileBuilder;
class TerrainTile;

template <class T> class TileFactory : public Factory <T, TileBuilder*>
{
	public:
		TileFactory(){}
		virtual ~TileFactory(){}
};

#endif