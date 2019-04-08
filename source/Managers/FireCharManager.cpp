#include "FireCharManager.h"
#include "../Tools/FireChar.h"
#include "MediaPathManager.h"

FireCharBuffer FireCharManager::charBuffer;

bool FireCharManager::loadXMLSettings(const char* path)
{
  const char *verified = MediaPathManager::lookUpMediaPath(path);
  XMLElement *particles = 0;
  XMLStack xmlStack;
  
  if (!verified)
    return Logger::writeErrorLog(String("Couldn't locate file at -> ") + path);
    
  if (!xmlStack.loadXMLFile(verified))
    return Logger::writeErrorLog(String("Invalid XML file -> ") + path);
    
  if (!(particles = xmlStack.getChildByName("FireCharSettings")))
    return Logger::writeErrorLog("Need a <FireCharSettings> root in the XML file");
    
  return loadXMLSettings(particles);
}

bool FireCharManager::loadXMLSettings(XMLElement* element)
{
  XMLElement *child = 0, *child2 = 0;
  
  if (element->getName() != "FireCharSettings")
    return Logger::writeErrorLog("Need a <FireCharSettings> root in the XML file");
    
  Tuple2f dotIntervals(1.5f, 1.5f);
  Tuple3f color;
  Texture texture;
  
  float maxSpread     = 0.2f;
  float miniLifeCycle = 0.5f;
  float startSize     = 1.0f;
  float endSize       = 1.0f;
  float delay         = 0.0f;
  float speed         = 20.0f;
  
  bool spin = false;
  int particlesPerDotCount = 15, warmUpIterations =  0;
  
  if (child = element->getChildByName("warmUpCount"))
    warmUpIterations = child->getValuei();
    
  if (child = element->getChildByName("particlesPerDot"))
    particlesPerDotCount = child->getValuei();
    
  if (child = element->getChildByName("maxSpread"))
    maxSpread = child->getValuef();
    
  if (child = element->getChildByName("lifeCycle"))
    miniLifeCycle = child->getValuef();
    
  if (child = element->getChildByName("speed"))
    speed = child->getValuef();
    
  if (child = element->getChildByName("spin"))
    spin = (child->getValue() == "true");
    
  if (child = element->getChildByName("delay"))
    delay = child->getValuef();
    
  if (child = element->getChildByName("Texture"))
    texture.loadXMLSettings(child);
    
  if (child = element->getChildByName("Color"))
    XMLElement::loadRX_GY_BZf(*child, color);
    
  if (child= element->getChildByName("DotIntervals"))
  {
    XMLElement::loadRX_GYf(*child, dotIntervals);
  }
  
  if (child = element->getChildByName("Size"))
  {
    if (child2 = child->getChildByName("start")) startSize = child2->getValuef();
    if (child2 = child->getChildByName("end"))   endSize   = child2->getValuef();
  }
  
  for (size_t i = 0; i < element->getChildrenCount(); i++)
  {
    if (!(child = element->getChild(i)))
      continue;
      
    const String &token = child->getName();
    
    if (token == "GridChar")
    {
      if (child2 = child->getChildByName("key"))
      {
        if (child2->getValuec() && !getFireChar(child2->getValuec()[0]))
        {
          FireChar *newFireChar = new FireChar(particlesPerDotCount, startSize, endSize, speed, delay, miniLifeCycle, maxSpread, spin);
          newFireChar->setDotIntervals(dotIntervals);
          if (newFireChar->setKeyChar(child))
          {
            newFireChar->setTexture(texture);
            newFireChar->setColor(color);
            addFireChar(newFireChar);
          }
          else
            deleteObject(newFireChar);
        }
      }
    }
  }
  
  for (int i = 0; i < warmUpIterations; i++)
    for (size_t t = 0; t < charBuffer.length(); t++)
      charBuffer[t]->updateLogic(getNextRandom());
      
  return true;
}

FireChar *FireCharManager::getFireChar(unsigned int index)
{
  return index >= charBuffer.length() ? 0 : charBuffer[index];
}

FireChar *FireCharManager::getFireChar(char character)
{
  for (unsigned int t = 0; t < charBuffer.length(); t++)
    if (charBuffer[t]->getKeyChar() == character)
      return charBuffer[t];
      
  return 0;
}

int FireCharManager::getCharIndex(char character)
{
  for (unsigned int t = 0; t < charBuffer.length(); t++)
    if (charBuffer[t]->getKeyChar() == character)
      return t;
      
  return -1;
}

bool FireCharManager::addFireChar(FireChar* fchar)
{
  if (fchar)
    if (!getFireChar(fchar->getKeyChar()))
    {
      fchar->initialize();
      charBuffer.append(fchar);
      return true;
    }
    
  return false;
}

bool FireCharManager::flushAll()
{
  charBuffer.clearAndDestroy();
  return true;
}