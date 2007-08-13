#ifndef BUILDER
#define BUILDER

class TerrainTile;

class Builder
{
	public:
		Builder(){}
		virtual ~Builder(){}
		virtual TerrainTile* Create() = 0;
};

#endif