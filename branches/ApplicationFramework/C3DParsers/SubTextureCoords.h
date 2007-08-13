#ifndef SUBTEXTURECOORDS
#define SUBTEXTURECOORDS 

#include "Math/Mathutils.h"

const int textureWidth  = 256;
const int textureHeight = 256;
const int tileWidth     = 64;
const int tileHeight    = 64;

namespace TileFlags
{
  enum TileQuad
  {
    QUAD0_DIAGONAL = 0x0001,
    QUAD1_DIAGONAL = 0x0002,
    QUAD2_DIAGONAL = 0x0004,
    QUAD3_DIAGONAL = 0x0008
  };

  enum TileTexture
  {
    TEXTURE_ROTATE90  = 0x0010,
    TEXTURE_ROTATE180 = 0x0020,
    TEXTURE_ROTATE270 = 0x0040,
    TEXTURE_MIRRORX   = 0x0080,
    TEXTURE_MIRRORY   = 0x0100
  };

  enum TileGeometry
  {
    DYNAMIC_MIX       = 0x0200,
    TILE_NOTESSELLATE = 0x0400,
    TILE_FLAT         = 0x0800,
    TILE_VISIBLE      = 0x8000
  };
}

class SubTextureCoords
{
  public:
     SubTextureCoords();
     SubTextureCoords(const SubTextureCoords &copy);
     SubTextureCoords(const Tuple4i &bounds);
     SubTextureCoords(int xOff, int yOff, int zOff, int wOff);
    ~SubTextureCoords();

  public:
    void           ComputeTextureCoords(const Tuple4i& bounds);
    void           ComputeTextureCoords(int x, int y, int z, int w);
    Tuple2f       *GetTextureCoords();
		void 					 TransformImage(unsigned int flags);
    void           TransformCoords(Tuple2f *coords);

  private:
    void           Rotate(float angle);

  private:
    Tuple2f textCoords[10];
};

#endif