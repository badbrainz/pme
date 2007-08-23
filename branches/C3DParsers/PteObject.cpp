#include "PteObject.h"
#include "Managers/MediaPathManager.h"
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#ifndef GL_BGR
#define GL_BGR GL_BGR_EXT
#endif

PteObject::PteObject()
{
  m_TexturePageCount  = 0;
  m_MipmapCount       = 0;
}

void PteObject::Flush()
{
  glDeleteTextures(m_TexturesID.length(), m_TexturesID.data());
  
  m_SubTextureCoords.clear();
  m_TexturesID.clear();
  
  m_TexturePageCount  = 0;
  m_MipmapCount       = 0;
}

void PteObject::GenerateMipmaps(unsigned char *imageData, int level, int width, int height)
{
  glTexImage2D(GL_TEXTURE_2D,
               level,
               3,
               width, height,
               0,
               GL_BGR,
               GL_UNSIGNED_BYTE,
               imageData);
}

void PteObject::SetupSubTextureCoords(void)
{
  int xPixelOffset = 0,
      yPixelOffset = 0;

  for(int y = 0; y < 4; y++, xPixelOffset = 0, yPixelOffset += tileHeight)
  for(int x = 0; x < 4; x++)
  {
    Tuple4i bounds(xPixelOffset, yPixelOffset, (xPixelOffset)+tileWidth, (yPixelOffset)+tileHeight);
    SubTextureCoords subCoords(bounds);
    m_SubTextureCoords.append(subCoords);
    xPixelOffset += tileWidth;
  }
}

SubTextureCoords &PteObject::CopySubTextureCoords(int index)
{
  return m_SubTextureCoords[index];
}

void PteObject::ActivateTexture(unsigned int id)
{
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, m_TexturesID[id]);
}

void PteObject::DeactivateTexture(void)
{
  glDisable(GL_TEXTURE_2D);
}

unsigned int PteObject::GetTexturePageCount()
{
  //return m_TexturePageCount;
  return m_TexturesID.length();
}

unsigned int PteObject::GetTexturePageID(unsigned int page)
{
  return m_TexturesID[page];
}

PteObject::~PteObject()
{
  //for(unsigned int i = 0; i < texturesID.length(); i++)
    glDeleteTextures(m_TexturesID.length(), m_TexturesID.data());
}
