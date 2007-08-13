#ifndef TILEDESCRIPTOR
#define TILEDESCRIPTOR

#include "Math/MathUtils.h"

class TileDescriptor
{
  public:
    unsigned int  primeTexture;
    unsigned int  seconTexture;
    Tuple2f      *texCoords;
};

#endif