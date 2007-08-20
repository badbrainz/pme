#ifndef TEXTURETILEBUILDER
#define TEXTURETILEBUILDER

#include "TileBuilder.h"

class Tile;

class TextureTileBuilder : public TileBuilder
{
	public:
		TextureTileBuilder();
		virtual ~TextureTileBuilder();
		virtual Tile* Create();
};

#endif