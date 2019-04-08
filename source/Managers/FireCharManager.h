#ifndef FIRECHARMANAGER
#define FIRECHARMANAGER

#include "../Containers/ocarray.h"

class XMLElement;
class FireChar;

typedef ArrayPtr <FireChar> FireCharBuffer;

class FireCharManager
{
  public:
    static bool flushAll();
    
    static bool loadXMLSettings(const char* path);
    static bool loadXMLSettings(XMLElement* element);
    
    static int getCharIndex(char seek);
    static bool addFireChar(FireChar* fchar);
    static FireChar *getFireChar(unsigned int index);
    static FireChar *getFireChar(char character);
    
  private:
    static FireCharBuffer charBuffer;
};

#endif
