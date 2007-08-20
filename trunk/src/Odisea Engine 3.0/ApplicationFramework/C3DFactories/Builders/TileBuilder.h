#ifndef TILEBUILDER
#define TILEBUILDER

class TerrainTile;

class TileBuilder
{
	public:
    TileBuilder(){}
    virtual ~TileBuilder(){}
		virtual TerrainTile* Create() = 0;
};

#endif