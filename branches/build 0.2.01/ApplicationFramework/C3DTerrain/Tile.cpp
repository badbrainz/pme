#include "Tile.h"

Tile::Tile()
{
  textureID[0]  = -1;
  textureID[1]  = -1;
  flags         = 0;
  maxHeight     = 0;
  //minHeight     = 0;
  extra         = 0;
}

void Tile::SetPrimaryTextureID(short id)
{
  textureID[0] = id;
}

void Tile::SetSecondaryTextureID(short id)
{
  textureID[1] = id;
}

void Tile::SetFlags(unsigned int bits)
{
  flags = bits;
}

void Tile::SetMaxHeight(unsigned char height)
{
  maxHeight = height;
}

/*void Tile::SetMinHeight(float height)
{
  minHeight = height;
}*/

void Tile::SetExtra(unsigned char data)
{
  extra = data;
}

short Tile::GetPrimaryTextureID(void)
{
  return textureID[0];
}

short Tile::GetSecondaryTextureID(void)
{
  return textureID[1];
}

unsigned int Tile::GetFlags(void)
{
  return flags;
}

unsigned char Tile::GetMaxHeight(void)
{
  return maxHeight;
}

/*float Tile::GetMinHeight(void)
{
  return minHeight;
}*/

unsigned char Tile::GetExtra(void)
{
  return extra;
}

Tile::~Tile()
{
}