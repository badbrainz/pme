#include "ManagersUtils.h"

vector<GeometryInfo*> GeometryManager::geometryCollection;

bool GeometryManager::addGeometryInfo(GeometryInfo *geometryInfo)
{
  if(!geometryInfo || !geometryInfo->getMedia())
    return Logger::writeErrorLog("Cannot add NULL GeometryInfo");
  GeometryManager::flushUnusedGeometry();
  geometryCollection.push_back(geometryInfo);
  return true;
}

GeometryInfo *GeometryManager::getGeometryInfo(const char* geometryPath)
{
  if(!geometryPath || !geometryCollection.size())
    return NULL;

  for(size_t i = 0; i < geometryCollection.size(); i++)
    if(geometryCollection[i]->getMediaPath() == geometryPath)
      return geometryCollection[i];

  return NULL;
}

GeometryInfo *GeometryManager::getGeometryInfo(unsigned int index)
{
  return (index >= geometryCollection.size()) ? NULL : geometryCollection[index];
}

void GeometryManager::flushUnusedGeometry()
{
  vector<GeometryInfo*> validGeometry;
  size_t i         = 0,
         count     = 0;

  for(i = 0; i < geometryCollection.size(); i++)
    if(geometryCollection[i]->getUserCount() > 0)
      validGeometry.push_back(geometryCollection[i]);
    else
    {
      count++;
      deleteObject(geometryCollection[i]->getMedia());
      deleteObject(geometryCollection[i]);
    }

  geometryCollection.clear();

  for(i = 0; i < validGeometry.size(); i++)
    geometryCollection.push_back(validGeometry[i]);

  if(count)
    Logger::writeInfoLog(String("Flushed Geometry count -> ") + int(count));
  Logger::flush();
}

void GeometryManager::printGeometryInfo()
{
  if(!geometryCollection.size())
    cout << "This Manager contains no geometries as of yet." << endl;
  else{
    cout << "Geometry list: " << endl;
    for(size_t i = 0; i < geometryCollection.size(); i++)
      cout <<  "<users = \"" << geometryCollection[i]->getUserCount() << "\" "
           <<  " path  = \"" << geometryCollection[i]->getMediaPath() << "\" />" << endl;
  }
}
