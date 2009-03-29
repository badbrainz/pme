#include "FireChar.h"
#include "../Managers/MediaPathManager.h"

Matrix4f FireChar::modelview;
Tuple3f  FireChar::right;
Tuple3f  FireChar::up;

FireChar::FireChar(int   particlesPerDot,
                   float startSize   , float endSize,
                   float speed       , float delay  , float mLifeCycle,
                   float maximumAngle, bool  textureSpin)
{
  dotIntervals.set(1.5f, 1.5f);
  initialized = false;
  keyChar = 0;
  
  setParticlesPerDotCount(particlesPerDot);
  setMaxSpreadOutAngle(maximumAngle);
  setMinimumLifeCycle(mLifeCycle);
  activateSpinning(textureSpin);
  
  setStartSize(startSize);
  setEndSize(endSize);
  setSpeed(speed);
  setDelay(delay);
  
  activeParticlesCount  = 0;
  particlesCount        = 0;
  previousCount         = 0;
  elapsedTime           = 0.0f;
  timer                 = 0.0f;
  height                = 0.0f;
  width                 = 0.0f;
}

bool FireChar::loadXMLSettings(const char *path)
{
  if (initialized)
    return Logger::writeErrorLog("FireChar already initialized");
    
  const char *verified   = MediaPathManager::lookUpMediaPath(path);
  XMLElement *particles  = 0;
  XMLStack    xmlStack;
  
  if (!verified)
    return Logger::writeErrorLog(String("Couldn't locate file at -> ") + path);
    
  if (!xmlStack.loadXMLFile(verified))
    return Logger::writeErrorLog(String("Invalid XML file -> ") + path);
    
  if (!(particles = xmlStack.getChildByName("FireChar")))
    return Logger::writeErrorLog("Need a <FireChar> root in the XML file");
    
  return loadXMLSettings(particles);
}

bool FireChar::loadXMLSettings(XMLElement *particles)
{
  if (initialized)
    return Logger::writeErrorLog("FireChar already initialized");
    
  XMLElement *child1 = 0, *child2 = 0;
                            
  if (particles->getName() != "FireChar")
    return Logger::writeErrorLog("Need a FireChar root in the XML file");
    
  for (size_t i = 0; i < particles->getChildrenCount(); i++)
  {
    if (!(child1 = particles->getChild(i)))
      continue;
      
    const String &token = child1->getName();
    
    if (token == "GridChar")
      setKeyChar(child1);
      
    if (token == "maxSpread")
      maximumAngle = child1->getValuef();
      
    if (token == "lifeCycle")
      setMinimumLifeCycle(child1->getValuef());
      
    if (token == "Size")
    {
      if (child2 = child1->getChildByName("start")) startSize = child2->getValuef();
      if (child2 = child1->getChildByName("end"))   endSize   = child2->getValuef();
    }
    
    if (token == "DotIntervals")
    {
      XMLElement::loadRX_GYf(*child1, dotIntervals);
      setDotIntervals(dotIntervals);
    }
    
    if (token == "speed")
      speed = child1->getValuef();
      
    if (token == "particlesPerDot")
      setParticlesPerDotCount(child1->getValuei());
      
    if (token == "Texture")
      texture.loadXMLSettings(child1);
      
    if (token == "delay")
      setDelay(child1->getValuef());
      
    if (token == "spin")
      activateSpinning((child1->getValue() == "true"));
      
    if (token == "Color")
      XMLElement::loadRX_GY_BZf(*child1, color);
  }

  return true;
}

bool FireChar::initialize()
{
  if (initialized || particlesCount <= 0)
    return Logger::writeErrorLog("FireChar already initialized or particle count is zero");
    
  if (keyChar == 0)
    return Logger::writeErrorLog("KeyChar is NULL");
    
  int singleSize = particlesCount * sizeof(float) * 1;
  int tripeSize  = particlesCount * sizeof(float) * 3;
  int quadSize   = particlesCount * sizeof(float) * 4;
  
  indices = new int[particlesCount * 4];
  for (int i = 0; i < particlesCount * 4; i++)
    indices[i] = i;
    
  spinDirections  = (float*) _aligned_malloc(singleSize, 16);
  lifeCycles      = (float*) _aligned_malloc(singleSize, 16);
  sizes           = (float*) _aligned_malloc(singleSize, 16);
  ages            = (float*) _aligned_malloc(singleSize, 16);
  
  memset(spinDirections, 0, singleSize);
  memset(lifeCycles,     0, singleSize);
  memset(sizes,          0, singleSize);
  memset(ages,           0, singleSize);
  
  directions  = (Tuple3f*) _aligned_malloc(tripeSize  , 16);
  texCoords   = (Tuple3f*) _aligned_malloc(tripeSize*4, 16);
  centers     = (Tuple3f*) _aligned_malloc(tripeSize  , 16);
  coords      = (Tuple3f*) _aligned_malloc(tripeSize*4, 16);
  colors      = (Tuple4f*) _aligned_malloc(quadSize *4, 16);
  speeds      = (Tuple3f*) _aligned_malloc(tripeSize  , 16);
  
  return (initialized = true);
}

void FireChar::updateLogic(float tick)
{
  if (!initialized)
    return;
    
  unsigned int t = 0;
  int i = 0;
  bool check = false;
  
  if (propagationDelay > elapsedTime)
  {
    elapsedTime += tick;
    activeParticlesCount = int((elapsedTime/propagationDelay) * particlesPerDot);
  }
  else
    activeParticlesCount = particlesPerDot;
    
  activeParticlesCount = clamp(activeParticlesCount, 0, particlesPerDot);
  
  for (t = 0; t < charLocations.length(); t++)
  {
    if (previousCount != activeParticlesCount)
    {
      for (i = previousCount; i< activeParticlesCount; i++)
        resetParticle(t * particlesPerDot + i);
      check = true;
    }
    
    for (i = 0; i< activeParticlesCount; i++)
      if (updateAndCheckState(tick, t * particlesPerDot + i))
        resetParticle(t * particlesPerDot + i);
  }
  
  previousCount = check ? activeParticlesCount : previousCount;
}

void FireChar::updateLocations(int index)
{
  int a = (index<<2);
  int b =  a + 1;
  int c =  a + 2;
  int d =  a + 3;
  
  coords[a] = coords[b] = coords[c] = coords[d] = right;
  
  coords[a] -= up;
  coords[a] *= sizes[index];
  coords[a] += centers[index];
  
  coords[b] += up;
  coords[b] *= sizes[index];
  coords[b] += centers[index];
  
  coords[c] -= up;
  coords[c] *=-sizes[index];
  coords[c] += centers[index];
  
  coords[d] += up;
  coords[d] *=-sizes[index];
  coords[d] += centers[index];
}

void FireChar::draw()
{
  if (!initialized)
    return;
    
  glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
  
  right.set(modelview[0], modelview[4], modelview[8]);
  up.set(modelview[1], modelview[5], modelview[9]);
  
  glDepthMask(GL_FALSE);
  
  for (int i = 0; i < particlesCount; i++)
    updateLocations(i);
    
  if (texture.getID())
  {
    texture.activate();
    glTexCoordPointer(3, GL_FLOAT, 0, texCoords);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  }
  
  glColorPointer(4, GL_FLOAT, 0, colors);
  glEnableClientState(GL_COLOR_ARRAY);
  
  glVertexPointer(3, GL_FLOAT, 0, coords);
  glEnableClientState(GL_VERTEX_ARRAY);
  
  if (activeParticlesCount == particlesCount)
    glDrawArrays(GL_QUADS, 0, particlesCount * 4);
  else
    for (unsigned int t = 0; t < charLocations.length(); t++)
      glDrawElements(GL_QUADS, activeParticlesCount * 4, GL_UNSIGNED_INT, indices + t * activeParticlesCount * 4);
      
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  
  if (texture.getID())
  {
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    texture.deactivate();
  }
  
  glDepthMask(GL_TRUE);
  glColor3f(1,1,1);
}

void FireChar::setParticlesPerDotCount(int ppd)
{
  if (initialized)
    Logger::writeErrorLog("Fire char already initialized");
  else
    particlesPerDot = clamp(ppd, 0, 1000);
}

void FireChar::setMinimumLifeCycle(float cycle)
{
  minimumLifeCycle = clamp(cycle, 0.1f, 10000.0f);
}

int FireChar::getParticlesPerDotCount()
{
  return particlesPerDot;
}

void FireChar::setMaxSpreadOutAngle(float angle)
{
  maximumAngle = clamp(angle, 0.0f, PI);
}

void FireChar::activateSpinning(bool active)
{
  spin = active;
}

void FireChar::setStartSize(float size)
{
  startSize = clamp(size, 0.01f, 10000.0f);
}

void FireChar::setEndSize(float size)
{
  endSize = clamp(size, 0.01f, 10000.0f);
}

void FireChar::setSpeed(float speedArg)
{
  speed = speedArg;
}

void FireChar::setDelay(float delay)
{
  propagationDelay = clamp(delay, 0.0f, 10000.0f);
}

char FireChar::getKeyChar()
{
  return keyChar;
}

void FireChar::setDotIntervals(const Tuple2f &xy)
{
  dotIntervals = xy;
}

void FireChar::setDotIntervals(float x, float y)
{
  dotIntervals.set(x, y);
}

bool FireChar::setKeyChar(XMLElement *gridChar)
{
  if (initialized)
    return Logger::writeErrorLog("Fire char already initialized");
    
  if (!gridChar || gridChar->getName() != "GridChar")
    return Logger::writeErrorLog("Need a GridChar tag");
    
  XMLElement *child, *rawData;
  Tuple2f charDots;
  
  if (!(child = gridChar->getChildByName("key")))
    return Logger::writeErrorLog("Need a <key> element in the tag");
    
  if (!(rawData = gridChar->getChildByName("RawData")))
    return Logger::writeErrorLog("Need a <RawData> element in the tag");
    
  if (rawData->rawData.byteCount != TOTAL_FIRE_CHAR_SIZE)
    return Logger::writeErrorLog(String("Size of the char grid bits is different than ") + TOTAL_FIRE_CHAR_SIZE);
    
  charLocations.clear();
  keyChar = child->getValuec() ? child->getValuec()[0] : 0;
  width = 0.0f;
  
  for (int y = 0; y < FIRE_CHAR_ROWS; y++)
  {
    for (int x = 0; x < FIRE_CHAR_COLUMNS; x++)
    {
      if (rawData->rawData.data[y*FIRE_CHAR_COLUMNS + x] == 49)
      {
        width = x > width ? x : width;
        charDots.set(dotIntervals.x * x, (y <= 9) ? dotIntervals.y * (9 - y) : (y == 10) ? -dotIntervals.y : -2.0f * dotIntervals.y);
        charLocations.append(charDots);
      }
      //    // cout << (rawData->rawData.data[y*FIRE_CHAR_COLUMNS + x] == 49);
    }
    //  //cout << endl;
  }
  
  particlesCount = charLocations.length() * particlesPerDot;
  height = FIRE_CHAR_ROWS*dotIntervals.y;
  width += 2;
  width *= dotIntervals.x;
  
  return true;
}

void FireChar::setTexture(const Texture & textureArg)
{
  texture = textureArg;
}

void FireChar::setTexture(const char * textureArg)
{
  texture.load2D(textureArg, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, false);
}

void FireChar::setColor(float r, float g, float b)
{
  color.set(clamp(r, 0.0f, 1.0f), clamp(g, 0.0f, 1.0f), clamp(b, 0.0f, 1.0f));
}

void FireChar::setColor(const Tuple3f &rgb)
{
  setColor(rgb.x, rgb.y, rgb.z);
}

int FireChar::getSourceIndex(int particleIndex)
{
  return (particleIndex /= particlesPerDot);
}

void FireChar::resetParticle(int index)
{
  static Tuple2f normalTexCoords[4] = {Tuple2f(0.0f, 1.0f), Tuple2f(1.0f, 1.0f),
                                       Tuple2f(1.0f, 0.0f), Tuple2f(0.0f, 0.0f)
                                      };
                                      
  int parentIndex = getSourceIndex(index);
  int newIndex = index << 2;
  
  for (int i = newIndex, j = 0; i < newIndex + 4; i++, j++)
  {
    texCoords[i].set(normalTexCoords[j].x, normalTexCoords[j].y, float(j));
    colors[i].set(color);
    coords[i].set(0,0,0);
  }
  
  sizes[index] = startSize;
  ages[index] = ages[index] - lifeCycles[index];
  
  lifeCycles[index] = minimumLifeCycle + minimumLifeCycle*getNextRandom() * 0.5f;
  centers[index].set(charLocations[parentIndex].x, charLocations[parentIndex].y, 0.0f);
  
  spinDirections[index] = getNextRandom() * 2.0f - 1.0f;
  
  speeds[index].set(0,0,0);
  generateSpeed(directions[index]);
  directions[index] *= speed;
}

bool FireChar::updateAndCheckState(float tick, int index)
{
  static Tuple2f  rotatingTexCoords[4] = {Tuple2f(-0.5f, 0.5f), Tuple2f(0.5f, 0.5f),
                                          Tuple2f(0.5f,-0.5f),Tuple2f(-0.5f,-0.5f)
                                         };
  float s = 0.0f;
  float c = 0.0f;
  float ratio = 0.0f;
  int newIndex = index << 2;
  size_t colIndex = 0;
  
  ages[index] += tick;
  ratio = clamp(ages[index]/lifeCycles[index], 0.0f, 1.0f);
  sizes[index] = startSize + (endSize - startSize) * ratio;
  centers[index] += speeds[index] * tick;
  speeds[index] += directions[index] * tick;
  
  if (spin)
  {
    s = fastSin(ages[index] * TWO_PI * spinDirections[index]);
    c = fastCos(ages[index] * TWO_PI * spinDirections[index]);
  }
  
  for (int i = 0; i < 4; i++)
  {
    if (spin)
    {
      texCoords[newIndex + i].x = rotatingTexCoords[i].x * s - rotatingTexCoords[i].y * c + 0.5f;
      texCoords[newIndex + i].y = rotatingTexCoords[i].x * c + rotatingTexCoords[i].y * s + 0.5f;
    }
    colors[newIndex + i] = color;
    colors[newIndex + i] *= 1.0f - ratio;
  }
  
  return (ratio >= 1.0f);
}

void FireChar::generateSpeed(Tuple3f &speed)
{
  float alphaAngle = 6.2831f * getNextRandom();
  float betaAngle  = 3.1415f * getNextRandom();
  
  betaAngle = betaAngle > maximumAngle ? maximumAngle : betaAngle;
  
  speed.z  = speed.x = fastSin(betaAngle);
  speed.x *= fastCos(alphaAngle);
  speed.y  = fastCos(betaAngle);
  speed.z *= fastSin(alphaAngle);
}

float FireChar::getHeight()
{
  return height;
}

float FireChar::getWidth()
{
  return width;
}

int FireChar::getParticlesCount()
{
  return particlesCount;
}

const Tuple3f &FireChar::getColor() const
{
  return color;
}

const Texture &FireChar::getTexture() const
{
  return texture;
}

float FireChar::getSpeed()
{
  return speed;
}

float FireChar::getMinimumLifeCycle()
{
  return minimumLifeCycle;
}

float FireChar::getDelay()
{
  return propagationDelay;
}

float FireChar::getMaxSpreadOutAngle()
{
  return maximumAngle;
}

bool FireChar::spinningActive()
{
  return spin;
}

float FireChar::getStartSize()
{
  return startSize;
}

float FireChar::getEndSize()
{
  return endSize;
}

const Tuple2f &FireChar::getDotIntervals() const
{
  return dotIntervals;
}

const CharDots *FireChar::getCharLocations() const
{
  return &charLocations;
}

FireChar::~FireChar()
{
  if (!initialized)
    return;
    
  deleteArray(indices);
  
  _aligned_free(spinDirections);
  _aligned_free(lifeCycles);
  _aligned_free(sizes);
  _aligned_free(ages);
  
  _aligned_free(directions);
  _aligned_free(texCoords);
  _aligned_free(centers);
  _aligned_free(coords);
  _aligned_free(colors);
  _aligned_free(speeds);
}