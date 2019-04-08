#ifndef FIRESTRING
#define FIRESTRING

#include "../Math/Tuple2.h"
#include "String.h"
#include <vector>

namespace FireStringAnchors
{
  enum Enum
  {
    TOP_LEFT,
    TOP_RIGHT,
    CENTERED
  };
}

typedef std::vector<int> Indices;
typedef std::vector<char> UpdateTracker;

class FireString
{
  private:
    void setIndices();
    
    UpdateTracker tracker;
    Indices       indices;
    String        charSet;
    bool          updateIndices;
    int           triangleCount;
    
  public:
    FireString(const char *string = NULL);
    FireString(const FireString &copy);
    FireString &operator =(const FireString &copy);
    
    void forceIndexUpdate();
    void updateTimers(float tick);
    void draw(int anchorPoint = FireStringAnchors::TOP_LEFT);
    
    void  setString(const char * charset);
    const String &getString() const;
    
    int     getTriangleCount();
    const   Indices *getCharIndices();
    Tuple2f getDimensions();
    float   getHeight();
    float   getWidth();
};

#endif
