#include "FireString.h"
#include "../Managers/FireCharManager.h"
#include "FireChar.h"
using namespace std;

FireString::FireString(const char *string)
{
  triangleCount =      0;
  updateIndices =   true;
  charSet       = string;
}

FireString::FireString(const FireString &copy)
{
  this->operator =(copy);
}

FireString &FireString::operator =(const FireString &copy)
{
  if (this != &copy)
  {
    updateIndices = copy.updateIndices;
    charSet       = copy.charSet;
    
    indices.clear();
    for (size_t i = 0; i < copy.indices.size(); i++)
      indices.push_back(copy.indices[i]);
  }
  return *this;
}

void FireString::forceIndexUpdate()
{
  updateIndices = true;
  setIndices();
}

void FireString::updateTimers(float tick)
{
  FireChar    *fChar = NULL;
  setIndices();
  tracker.clear();
  bool byPass = false;
  
  for (size_t i = 0; i < indices.size(); i++)
  {
    byPass = false;
    if (fChar = FireCharManager::getFireChar(size_t(indices[i])))
    {
      for (size_t c = 0; c < tracker.size(); c++)
        if (fChar->getKeyChar() == tracker[c])
        {
          byPass = true;
          break;
        }
        
      if (!byPass)
      {
        fChar->updateLogic(tick);
        tracker.push_back(fChar->getKeyChar());
      }
    }
  }
}

void FireString::setIndices()
{
  if (updateIndices && charSet.getLength())
  {
    indices.clear();
    indices.resize(charSet.getLength());
    
    for (size_t i = 0; i < indices.size(); i++)
    {
      indices[i] = FireCharManager::getCharIndex(charSet[i]);
      if ((indices[i] == -1) && (charSet[i] == ' '))
        indices[i] = -2;
    }
    updateIndices = false;
  }
}

void FireString::draw(int anchorPoint)
{
  triangleCount = 0;
  FireChar *currentChar   = NULL;
  float     xOffset       = 0.0f, yOffset = 0.0f;
  
  setIndices();
  glPushMatrix();
  
  switch (anchorPoint)
  {
    case FireStringAnchors::CENTERED:
      xOffset = getWidth();
      xOffset/=-2.0f;
      break;
    case FireStringAnchors::TOP_LEFT:
      yOffset = -getHeight();
      break;
    case FireStringAnchors::TOP_RIGHT:
      yOffset = -getHeight();
      xOffset =  -getWidth();
      break;
  }
  
  glTranslatef(xOffset, yOffset, 0.0f);
  
  for (size_t i = 0; i < indices.size(); i++)
    if (currentChar = FireCharManager::getFireChar(size_t(indices[i])))
    {
      currentChar->draw();
      xOffset = currentChar->getWidth();
      glTranslatef(xOffset, 0.0, 0.0);
      triangleCount += currentChar->getParticlesCount() * 2;
    }
    else
    {
      if (indices[i] == -2.0)
        glTranslatef(4.0f, 0.0, 0.0);
    }
    
  glPopMatrix();
}

void  FireString::setString(const char * charset)
{
  setIndices();
  updateIndices = true;
  charSet       = charset;
}

const String  &FireString::getString() const
{
  return charSet;
}

const   Indices *FireString::getCharIndices()
{
  return &indices;
}

Tuple2f FireString::getDimensions()
{
  return Tuple2f(getWidth(), getHeight());
}

int FireString::getTriangleCount()
{
  return triangleCount;
}

float FireString::getHeight()
{
  float height = 0.0f;
  
  FireChar *currentChar = NULL;
  for (size_t i = 0; i < indices.size(); i++)
    if (currentChar = FireCharManager::getFireChar(size_t(indices[i])))
    {
      height  =  height < currentChar->getHeight() ? currentChar->getHeight() : height;
    }
  return height;
}

float FireString::getWidth()
{
  float width = 0.0f;
  
  FireChar *currentChar = NULL;
  
  for (size_t i = 0; i < indices.size(); i++)
    if (currentChar = FireCharManager::getFireChar(size_t(indices[i])))
      width += currentChar->getWidth();
      
  for (size_t t = 0; t < charSet.getLength(); t++)
    if (charSet[t] == ' ')
      width += 4.0f;
      
  return width;
}