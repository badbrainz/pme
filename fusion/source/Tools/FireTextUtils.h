#ifndef FIRETEXTUTILS
#define FIRETEXTUTILS

#include "FireString.h"
#include "../Containers/ocarray.h"

struct FireStringsGroup
{
  String title;
  Array <String> titleHolder;
};

class FSGIterator
{
  private:
    FireStringsGroup* currentFSG;
    unsigned int counter;

  public:
    FSGIterator();
    ~FSGIterator();

  public:
    const char* getTitle();
    const char* getMember();
    const char* getNextMember();
    void setFireStringGroup(FireStringsGroup* group);
    bool end();
};

class FireStringDB
{
  private:
    FireString title, titleHolder;
    FSGIterator iterator;
    Array <FireStringsGroup> fireStringTable;
    unsigned int counter;
    __int64 speed;

  public:
    FireStringDB();
    ~FireStringDB();

  public:
    void initialize();
    void reset();
    void update(float tick);
    void draw();
    bool end();

  private:
    void rewindInterator();
};

#endif