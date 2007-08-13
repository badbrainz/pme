#ifndef TERRAINTILE
#define TERRAINTILE

#include "Math/MathUtils.h"

class Tile
{
  public:
     Tile();
    ~Tile();

  public:
    void            SetPrimaryTextureID(short id);
    void            SetSecondaryTextureID(short id);
    void            SetFlags(unsigned int bits);
    void            SetMaxHeight(unsigned char height);
    //void            SetMinHeight(float height);
    void            SetExtra(unsigned char data);
    
    short           GetPrimaryTextureID(void);
    short           GetSecondaryTextureID(void);
    unsigned int    GetFlags(void);
    unsigned char   GetMaxHeight(void);
    //float           GetMinHeight(void);
    unsigned char   GetExtra(void);
  
  private:
    short           textureID[2];
    unsigned short  flags;
    unsigned char		maxHeight;
    unsigned char   extra;
};

#endif