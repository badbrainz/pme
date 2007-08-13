#include "Configuration.h"
#include "Managers/MediaPathManager.h"

Configuration::Configuration() : IOXMLObject("Configuration")
{
}

bool Configuration::Initialize()
{
  if (!IOXMLObject::loadXMLSettings(String(ioname + ".xml")))
    return Logger::writeErrorLog(String("could not initialize ") + ioname);
  
  return true;
}

bool Configuration::loadXMLSettings(XMLElement *element)
{
  if (!isSuitable(element))
    return false;

  XMLElement *child = 0;
  
  for (size_t i = 0; i < element->getChildrenCount(); i++)
  {
    child = element->getChild(i);
    if (child->getName() == "MediaFolder")
    {
      MediaPathManager::registerPath(child);
      continue;
    }
  }
  
  return true;
}


Configuration::~Configuration()
{
}