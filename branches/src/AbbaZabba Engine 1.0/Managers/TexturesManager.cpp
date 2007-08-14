#include "TexturesManager.h"

vector<TextureInfo*> TexturesManager::textureCollection;

bool TexturesManager::addTextureInfo(TextureInfo *textureInfo)
{
  flushUnusedTextures();

  if(!textureInfo)
    return false;
  textureCollection.push_back(textureInfo);
  return true;
}

TextureInfo *TexturesManager::getTextureInfo(const char* texturePath)
{
  flushUnusedTextures();
  if(!texturePath || !textureCollection.size())
    return NULL;

  for(size_t i = 0; i < textureCollection.size(); i++)
    if(textureCollection[i]->getMediaPath() == texturePath)
      return textureCollection[i];

  return NULL;
}

TextureInfo *TexturesManager::getTextureInfo(GLuint textureID)
{
  if(!textureID || !textureCollection.size())
    return NULL;

  for(size_t i = 0; i < textureCollection.size(); i++)
    if(textureCollection[i]->getMedia() == textureID)
      return textureCollection[i];

  return NULL;
}

void TexturesManager::flushUnusedTextures()
{
  vector<TextureInfo*> validTextures;
  size_t i         = 0,
         count     = 0;
  GLuint textureID = 0;

  for(i = 0; i < textureCollection.size(); i++)
    if(textureCollection[i]->getUserCount() > 0)
    {
      validTextures.push_back(textureCollection[i]);
    }
    else
    {
      count++;
      textureID = textureCollection[i]->getMedia();
      glDeleteTextures(1, &textureID);
      deleteObject(textureCollection[i]);
    }

  textureCollection.clear();

  for(i = 0; i < validTextures.size(); i++)
      textureCollection.push_back(validTextures[i]);

  if(count)
    Logger::writeInfoLog(String("Flushed textures count -> ") + int(count));
}

void TexturesManager::flushAllTextures(){
  GLuint textureID = 0;
  size_t i;

  for(i = 0; i < textureCollection.size(); i++)
  {
    textureID = textureCollection[i]->getMedia();
    glDeleteTextures(1, &textureID);
    deleteObject(textureCollection[i]);
  }

  textureCollection.clear();
  if(i) Logger::writeInfoLog("Flushed all active textures");
}

void TexturesManager::printTexturesInfo()
{
  if(!textureCollection.size())
    cout << "This Manager contains no Textures as of yet." << endl;
  else{
    cout << "Textures list: " << endl;
    for(size_t i = 0; i < textureCollection.size(); i++)
      cout <<  "<users = \"" << textureCollection[i]->getUserCount() << "\" "
           <<  " path  = \"" << textureCollection[i]->getMediaPath() << "\" "
           <<  " id    = \"" << textureCollection[i]->getMedia()     << "\" />" << endl;
  }
}
