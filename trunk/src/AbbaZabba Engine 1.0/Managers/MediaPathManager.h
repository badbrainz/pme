#ifndef MEDIAPATHMANAGER_H
#define MEDIAPATHMANAGER_H

#include "../Tools/Logger.h"
#include <vector>

using namespace std;

class MediaPathManager
{
  public:
    static int          getPathCount();
    static bool         registerPath(XMLElement *MediaPathNode);
    static bool         registerPath(const char *path);
    static void         printAllPaths();
    static const char*  lookUpMediaPath(const char *path);
  private:
    static vector<String> pathStack;
};

#endif