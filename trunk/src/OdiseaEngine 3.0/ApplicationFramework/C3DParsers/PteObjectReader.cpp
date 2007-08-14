#include "PteObject.h"
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#ifndef GL_BGR
#define GL_BGR GL_BGR_EXT
#endif

bool PteObject::LoadFromFile(const String &filePath)
{
  m_InputFile.open(filePath, std::ofstream::in|std::ofstream::binary);

  ReadHeader();
  for(unsigned int i = 0; i < m_TexturePageCount; i++)
  {
    ReadPrimaryTexturePageChunk();
    SkipMipMaps();
  }

  SetupSubTextureCoords();

  m_InputFile.close();

  return true;
}

void PteObject::ReadHeader(void)
{
  m_InputFile.read((char*)&m_TexturePageCount, 4);
  m_InputFile.seekg(8, std::ios::cur);
  m_InputFile.read((char*)&m_MipmapCount, 4);
}

void PteObject::ReadPrimaryTexturePageChunk(void)
{
  int imageWidth    = 256;
  int imageHeight   = 256;
  int elementsCount = imageWidth * imageHeight * 3;
  unsigned int availableID;
  unsigned char *dataBuffer;

  dataBuffer = new unsigned char[elementsCount];
  std::fill(dataBuffer, dataBuffer+elementsCount, 0);
  m_InputFile.read((char*)dataBuffer, elementsCount);

  glGenTextures(1, &availableID);
  m_TexturesID.append(availableID);
  //texturesID.push_back(availableID);

  glBindTexture(GL_TEXTURE_2D, availableID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_NEAREST);

  glTexImage2D(GL_TEXTURE_2D,               // target
               0,                           // level-of-detail
               3,                           // number of color components
               imageWidth, imageHeight,     // texture dimensions
               0,                           // border width
               GL_BGR,                      // pixel format
               GL_UNSIGNED_BYTE,            // data type
               dataBuffer);                 // pointer to the image data

  gluBuild2DMipmaps(GL_TEXTURE_2D,
                    3,
                    imageWidth, imageHeight,
                    GL_BGR,
                    GL_UNSIGNED_BYTE,
                    dataBuffer);

  delete [] dataBuffer;
}

void PteObject::ReadMipmaps(void)
{
  int imageWidth    = 256;
  int imageHeight   = 256;
  int elementsCount;
  int resolution = 2;
  int level = 1;
  unsigned char *dataBuffer = new unsigned char[ (imageWidth/resolution) * (imageHeight/resolution) * 3 ];

  for(unsigned int i = 0; i < m_MipmapCount; i++)
  {
    elementsCount = (imageWidth/resolution) * (imageHeight/resolution) * 3;
    //dataBuffer = new unsigned char[ elementsCount ];
    std::fill(dataBuffer, dataBuffer+elementsCount, 0);
    m_InputFile.read((char*)dataBuffer, elementsCount);
    GenerateMipmaps(dataBuffer, level++, (imageWidth/resolution), (imageHeight/resolution));
    resolution *= 2;
    //delete [] dataBuffer;
    //dataBuffer = 0;
  }
  delete [] dataBuffer;
}

void PteObject::SkipMipMaps(void)
{
  int imageWidth    = 256;
  int imageHeight   = 256;
  int resolution    = 2;

  for(unsigned int i = 0; i < m_MipmapCount; i++, resolution *= 2)
    m_InputFile.seekg((imageWidth/resolution)*(imageHeight/resolution)*3, std::ios::cur);
}