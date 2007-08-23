#ifndef PTEOBJECT
#define PTEOBJECT

#include "SubTextureCoords.h"
#include "Tools/Logger.h"
#include "../OC/ocarray.h"
#include <fstream>

class PteObject
{
  public:
     PteObject();
    ~PteObject();

  public:
    bool          			LoadFromFile(const String &filePath);
    SubTextureCoords	 &CopySubTextureCoords(int index);
    void          			ActivateTexture(unsigned int id);
    void          			DeactivateTexture(void);
    
    unsigned int        GetTexturePageCount();
    unsigned int        GetTexturePageID(unsigned int page);
    
    void Flush();

  private:
    void          			ReadHeader(void);
    void          			ReadPrimaryTexturePageChunk(void);
    void          			ReadMipmaps(void);
    void          			SkipMipMaps(void);
    
    void          			SetupSubTextureCoords(void);
    void          			GenerateMipmaps(unsigned char *imageData, int level, int width, int height);

  private:
    unsigned int              m_TexturePageCount,
                              m_MipmapCount;
    
    std::ifstream		          m_InputFile;
    
    Array <unsigned int>      m_TexturesID;
    Array <SubTextureCoords>  m_SubTextureCoords;
};

#endif