#include "FireTextUtils.h"

FSGIterator::FSGIterator()
{
  currentFSG = 0;
  counter = 0;
}

const char* FSGIterator::getTitle()
{
  return currentFSG->title;
}

const char* FSGIterator::getMember()
{
  return currentFSG->titleHolder[counter];
}

const char* FSGIterator::getNextMember()
{
  return currentFSG->titleHolder[counter++];
}

void FSGIterator::setFireStringGroup(FireStringsGroup* group)
{
  currentFSG = group;
  counter = 0;
}

bool FSGIterator::end()
{
  return (counter == currentFSG->titleHolder.length());
}

FSGIterator::~FSGIterator()
{
}
