#include "TextureTileBuilder.h"
#include "../../C3DTerrain/Tile.h"

TextureTileBuilder::TextureTileBuilder()
{}

TextureTileBuilder::~TextureTileBuilder()
{}

Tile* TextureTileBuilder::Create()
{
	return new Tile;
}