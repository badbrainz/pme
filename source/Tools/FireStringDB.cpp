#include "FireTextUtils.h"
#include "Timer.h"
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

FireStringDB::FireStringDB()
{
  reset();
}

void FireStringDB::initialize()
{
  FireStringsGroup group;
  
  group.title = "DIRECTOR";
  group.titleHolder.clear();
  group.titleHolder.append("Awakening");
  fireStringTable.append(group);
  
  group.title = "LEAD PROGRAMMER";
  group.titleHolder.clear();
  group.titleHolder.append("kwAnTUM");
  fireStringTable.append(group);
  
  group.title = "PROGRAMMERS";
  group.titleHolder.clear();
  group.titleHolder.append("diabloprimero");
  group.titleHolder.append("Mark L.");
  fireStringTable.append(group);
  
  group.title = "GRAPHICS";
  group.titleHolder.clear();
  group.titleHolder.append("Devoti");
  fireStringTable.append(group);
  
  group.title = "MODS";
  group.titleHolder.clear();
  group.titleHolder.append("Imperial Studio");
  group.titleHolder.append("A-Team");
  group.titleHolder.append("WoG Studio");
  group.titleHolder.append("Visual Studio");
  fireStringTable.append(group);

  group.title = "SPECIAL THANKS";
  group.titleHolder.clear();
  group.titleHolder.append("Abdul Bezrati");
  fireStringTable.append(group);
  
  rewindInterator();
}

void FireStringDB::update(float tick)
{
  title.updateTimers(tick);
  titleHolder.updateTimers(tick);

  if (2.5f <= Timer::getElapsedTimeSeconds(speed))
  {
    if (iterator.end())
    {
      ++counter;
      if (end())
        return;
      iterator.setFireStringGroup(&fireStringTable[counter]);
      titleHolder.setString(iterator.getMember());
      title.setString(iterator.getTitle());
      titleHolder.forceIndexUpdate();
      title.forceIndexUpdate();
    }
    else
    {
      titleHolder.setString(iterator.getNextMember());
      titleHolder.forceIndexUpdate();
    }

    speed = Timer::getCurrentTime();
  }
}

void FireStringDB::draw()
{
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslatef(0,30,10);
  title.draw(FireStringAnchors::CENTERED);
  glPopMatrix();
  titleHolder.draw(FireStringAnchors::CENTERED);
}

void FireStringDB::reset()
{
  counter = 0;
  speed = Timer::getCurrentTime();
  if (fireStringTable.entries()) rewindInterator();
}

void FireStringDB::rewindInterator()
{
  iterator.setFireStringGroup(&fireStringTable[0]);
  title.setString(iterator.getTitle());
  titleHolder.setString(iterator.getMember());
}

bool FireStringDB::end()
{
  return (counter == fireStringTable.length());
}

FireStringDB::~FireStringDB()
{
}
