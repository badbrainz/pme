#include "Particles.h"
#include "../Geometry/ShadowInfo.h"
#include "../Renderer/Renderer.h"
#include <xmmintrin.h>

Matrix4f Particles::modelview;
Tuple3f  Particles::right;
Tuple3f  Particles::up;

Particles::Particles(int   numPart,
                     float startSizeArg,
                     float endSizeArg,
                     float speedArg,
                     float delay,
                     float minimumLifeTArg,
                     float maximumAngleArg,
                     float accelerationArg,
                     bool  spinArg,
                     bool  fadeArg)
{
  activeParticlesCount = 0;
  propagationDelay     = delay;
  minimumLifeCycle     = minimumLifeTArg;
  particlesCount       = numPart;
  sortedIndices        = NULL;
  previousCount        = 0;
  acceleration         = accelerationArg;
  maximumAngle         = maximumAngleArg;
  elapsedTime          = 0.0f;
  initialized          = false;
  collisionOn          = true;
  startSize            = startSizeArg;
  endSize              = endSizeArg; 
  timer                = 0.0f;
  speed                = speedArg;
  range                = 100.0f;
  spin                 = spinArg;
  fade                 = fadeArg;
}

void Particles::initialize()
{
  if(initialized || particlesCount <= 0)
    return;

  int singleSize   = particlesCount * sizeof(float) * 1,
      tripeSize    = particlesCount * sizeof(float) * 3;

  spinDirections   = (float*) _aligned_malloc(singleSize, 16);
  lifeCycles       = (float*) _aligned_malloc(singleSize, 16);
  sizes            = (float*) _aligned_malloc(singleSize, 16);
  ages             = (float*) _aligned_malloc(singleSize, 16);

  memset(spinDirections, 0, singleSize);
  memset(lifeCycles,     0, singleSize);
  memset(sizes,          0, singleSize);
  memset(ages,           0, singleSize);

  initialCenters = (Tuple3f*)_aligned_malloc(tripeSize  , 16);
  directions     = (Tuple3f*)_aligned_malloc(tripeSize  , 16);
  texCoords      = (Tuple3f*)_aligned_malloc(tripeSize*4, 16);
  centers        = (Tuple3f*)_aligned_malloc(tripeSize  , 16);
  coords         = (Tuple3f*)_aligned_malloc(tripeSize*4, 16);
  colors         = (Tuple3f*)_aligned_malloc(tripeSize*4, 16);

  initialized = true;
}

bool Particles::loadXMLSettings(const char*xmlPath)
{
  if(initialized)
    return false;

  const char *verified   = MediaPathManager::lookUpMediaPath(xmlPath);
  XMLElement *particles  = NULL;
  XMLStack    xmlStack;
  
  if(!verified)
    return Logger::writeErrorLog(String("Couldn't locate file at -> ") + xmlPath);

  if(!xmlStack.loadXMLFile(verified))
    return Logger::writeErrorLog(String("Invalid XML file -> ") + xmlPath);

  if(!(particles = xmlStack.getChildByName("Particles")))
    return Logger::writeErrorLog("Need a Particle root in the XML file");

  return loadXMLSettings(particles);
}

bool Particles::loadXMLSettings(XMLElement *particles)
{
  if(initialized)
    return false;

  XMLElement *child1      = NULL,
             *child2      = NULL;

  if(particles->getName() != "Particles")
    return Logger::writeErrorLog("Need a Particles root in the XML file");


  if(child1 = particles->getChildByName("description"))
    return loadXMLSettings(child1->getValuec());

  for(size_t i = 0; i < particles->getChildrenCount(); i++)
  {
    if(!(child1 = particles->getChild(i)))
      continue;
    
    const String &token = child1->getName();

    if(token == "ColorSpline") 
      colorSpline.loadXMLOrbit(child1->getChildByName("Spline"));

    if(token == "PathSpline") 
      pathSpline.loadXMLOrbit(child1->getChildByName("Spline"));

    if(token == "maxSpread") 
      maximumAngle = child1->getValuef();

    if(token == "lifeTime" || token == "lifeCycle") 
      minimumLifeCycle = child1->getValuef();

    if(token == "Size") 
    {
      if(child2 = child1->getChildByName("start")) startSize = child2->getValuef();
      if(child2 = child1->getChildByName("end"))   endSize   = child2->getValuef();
    }

    if(token == "AlphaAttributes")
    {
      alphaAttr.loadXMLSettings(child1);
      continue;
    }

    if(token == "BlendAttributes")
    {
      blendAttr.loadXMLSettings(child1);
      continue;
    }

    if(token == "speed") 
      speed = child1->getValuef();

    if(token == "acceleration") 
      acceleration = child1->getValuef();

    if(token == "range") 
      range = clamp(child1->getValuef(), 1.0f, 10000.0f);

    if(token == "count") 
      particlesCount = child1->getValuei();

    if(token == "Texture") 
      texture.loadXMLSettings(child1);

    if(token == "delay") 
      propagationDelay = child1->getValuef();

    if(token == "spin") 
      spin = (child1->getValue() == "true");

    if(token == "fade") 
      fade = (child1->getValue() == "true");

    if(token == "progress") 
      timer = child1->getValuef();

    if(token == "Color") 
      XMLElement::loadRX_GY_BZf(*child1, color);

    if(token == "Force" || token == "ExternalForce" ) 
      XMLElement::loadRX_GY_BZf(*child1, externalForce);

    if((token == "Center") || (token == "Position"))
      XMLElement::loadRX_GY_BZf(*child1, center);
  }
  initialize();
  return true;
}

void Particles::updateLogic(float tick)
{
  if(!initialized)
    return;

  static Tuple3f newPosition;
  register int i = 0;

  timer  += tick;
  timer  -= (timer >= pathSpline.getCycleLength()) ? pathSpline.getCycleLength() : 0.0f;

  newPosition  = pathSpline.getLocationAtTime(timer);

  if(newPosition.getLengthSquared())
    center.set(newPosition);

  if(propagationDelay > elapsedTime)
  {
    elapsedTime += tick;
    activeParticlesCount = int((elapsedTime/propagationDelay)* particlesCount);
  }
  else
    activeParticlesCount = particlesCount;

  activeParticlesCount = clamp(activeParticlesCount, 0, particlesCount);

  if(previousCount != activeParticlesCount)
  {
    for(i = previousCount; i< activeParticlesCount; i++)
      resetParticle(i);
    previousCount = activeParticlesCount;
  }

  for(i = 0; i< activeParticlesCount; i++)
    if(updateAndCheckState(tick, i))
      resetParticle(i);
}

bool Particles::updateAndCheckState(float tick, int index)
{
  static Tuple2f  rotatingTexCoords[4] = {Tuple2f(-0.5f, 0.5f), Tuple2f( 0.5f, 0.5f),
                                          Tuple2f( 0.5f,-0.5f),Tuple2f(-0.5f,-0.5f)};
  float  realTime = 0.0f,
         ratio    = 0.0f,
         s        = 0.0f,
         c        = 0.0f;
        
  int    newIndex = index<<2;

  size_t colIndex = 0;

  ages[index]    += tick;
  realTime        = ages[index]*speed;
  centers[index]  = initialCenters[index];
  centers[index] += directions[index]*realTime;
  centers[index] += directions[index]*acceleration*0.5f*realTime*realTime;
  centers[index] += externalForce*tick;

  ratio           = clamp(ages[index]/lifeCycles[index], 0.0f, 1.0f);
  sizes[index]    = startSize + (endSize - startSize)*ratio;
 
  if(collisionOn)
  {
    Tuple3f difference;

    for(colIndex = 0; colIndex < collisionSpheres.size(); colIndex++)
    {
     
     difference  = centers[index];
     difference -= *(Tuple3f*)&collisionSpheres[colIndex].x;
      
      if(difference.getLength() <= sizes[index] + collisionSpheres[colIndex].w)
      {
        difference.normalize();
        centers[index]        = *(Tuple3f*)&collisionSpheres[colIndex].x;
        centers[index]       += difference*(collisionSpheres[colIndex].w + sizes[index]);
      } 
    }
  }
 
  if(spin)
  {
    s = fastSin(ages[index] * TWO_PI * spinDirections[index] );
    c = fastCos(ages[index] * TWO_PI * spinDirections[index] );
  }

  Tuple3f newColor3f = colorSpline.getLocationAtTime(ratio*colorSpline.getCycleLength());

  if(!newColor3f.getLengthSquared())
    newColor3f = color;

  for(int i = 0; i < 4; i++)
  {
    if(spin)
    {
      texCoords[newIndex + i].x = rotatingTexCoords[i].x * s - rotatingTexCoords[i].y * c + 0.5f;
      texCoords[newIndex + i].y = rotatingTexCoords[i].x * c + rotatingTexCoords[i].y * s + 0.5f;
    }
    colors[newIndex + i]  = newColor3f;
    if(fade)
      colors[newIndex + i] *= clamp(1.0f - ratio, 0.0f, 1.0f);
  }
   
  return (ratio >= 1.0f - EPSILON);
}

void Particles::updateLocations(int index)
{
  int a = (index<<2),
      b =  a + 1,
      c =  a + 2,
      d =  a + 3;

  coords[a]  = 
  coords[b]  = 
  coords[c]  = 
  coords[d]  = right;

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

bool Particles::sort(int order, float x, float y, float z)
{
  DistanceObject<int>  quandInfo;
  float           coeff    = (order == FRONT_TO_BACK) ? 1.0f :
                             (order == BACK_TO_FRONT) ?-1.0f : 0.0f;
  int             index    = 0,
                  i        = 0;

  if(!coeff)
    return false;

  sortedIndices        = !sortedIndices ? new int[particlesCount*4] : sortedIndices;

  if(!distanceProcessor.size())
  {
    for(i = 0; i < particlesCount; i++)
    {
      quandInfo.setDistance(coeff*((centers[i].x - x) * (centers[i].x - x) +
                                   (centers[i].y - y) * (centers[i].y - y) +
                                   (centers[i].z - z) * (centers[i].z - z)));
      quandInfo.setObject(i);
      distanceProcessor.push_back(quandInfo);
    }
  }
  else
  {
    for(i = 0; i < particlesCount; i++)
    {
      distanceProcessor[i].setDistance(coeff*((centers[i].x - x) * (centers[i].x - x) +
                                              (centers[i].y - y) * (centers[i].y - y) +
                                              (centers[i].z - z) * (centers[i].z - z)));
      distanceProcessor[i].setObject(i);
    }
  }
  ::sort(distanceProcessor.begin(), distanceProcessor.end());
 
  i  = 0;
  for(size_t t = 0; t < distanceProcessor.size(); t++)
  {
    index               = distanceProcessor[t].getObject()*4;
    sortedIndices[i++] = index + 0;
    sortedIndices[i++] = index + 1;
    sortedIndices[i++] = index + 2;
    sortedIndices[i++] = index + 3;
  }

  return true;
}

void Particles::draw(int order, const Tuple3f &camPosition)
{
  draw(order, camPosition.x, camPosition.y, camPosition.z);
}

void Particles::draw(int order, float x, float y, float z)
{
  if(!initialized)
    return;
 
  bool proccessSorted = sort(order, x, y, z);

  glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

  right.set(modelview[0], modelview[4], modelview[8]);
  up.set   (modelview[1], modelview[5], modelview[9]);
 
  for(int i = 0; i < activeParticlesCount; i++)
    updateLocations(i);

  glDepthMask(GL_FALSE);
  blendAttr.apply();
  alphaAttr.apply();

  if(texture.getID())
  {
    texture.activate();
    glTexCoordPointer(3, GL_FLOAT, 0, texCoords);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  }

  glColorPointer(3, GL_FLOAT, 0, colors);
  glEnableClientState(GL_COLOR_ARRAY);

  glVertexPointer(3, GL_FLOAT, 0, coords);
  glEnableClientState(GL_VERTEX_ARRAY);

  if(!proccessSorted)
    glDrawArrays(GL_QUADS, 0, activeParticlesCount*4);
  else
   glDrawElements(GL_QUADS, activeParticlesCount*4, GL_UNSIGNED_INT, sortedIndices);
   
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
 
  if(texture.getID())
  {
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    texture.deactivate();
  }
  glDepthMask(GL_TRUE);

  blendAttr.stop();
  alphaAttr.stop();

  glColor3f(1,1,1);
}

void Particles::resetParticle(int index)
{
  static Tuple2f normalTexCoords[4] = {Tuple2f(0.0f, 1.0f), Tuple2f(1.0f, 1.0f),
                                       Tuple2f(1.0f, 0.0f), Tuple2f(0.0f, 0.0f)};
  int newIndex = index << 2;
 
  for(int i = newIndex, j = 0; i < newIndex + 4; i++, j++)
  {
    texCoords[i].set(normalTexCoords[j].x,
                                    normalTexCoords[j].y,
                                    float(j));
    colors[i].set(color);
    coords[i].set(0,0,0);
  }

  ages[index]             =  0.0f;
  sizes[index]            =  startSize;
  centers[index]          =  center;
  lifeCycles[index]       =  minimumLifeCycle + minimumLifeCycle*getNextRandom()*0.5f;
  initialCenters[index]   =  center;
  spinDirections[index]   = -getNextRandom() + getNextRandom();
  getNewDirection(directions[index]);
}

void  Particles::enableCollision(bool on )
{
  collisionOn = on;
}

bool  Particles::collisionEnabled()
{
  return collisionOn;
}

void  Particles::addCollisionSphere(const Tuple4f &collSphere)
{
  collisionSpheres.push_back(collSphere);
}

void Particles::getNewDirection(Tuple3f &direction)
{
  float alphaAngle = 6.2831f*getNextRandom(),
        betaAngle  = 3.1415f*getNextRandom();

  betaAngle = betaAngle > maximumAngle ? maximumAngle : betaAngle;

  direction.z  = direction.x = fastSin(betaAngle);
  direction.x *= fastCos(alphaAngle);
  direction.y  = fastCos(betaAngle);
  direction.z *= fastSin(alphaAngle);
}

void  Particles::setMinimumLifeCycle(float cycle)
{
  minimumLifeCycle = clamp(cycle, 0.1f, 10000.0f);
}

float Particles::getMinimumLifeCycle()
{
  return minimumLifeCycle;
}

void  Particles::setDelay(float delay)
{
  propagationDelay = clamp(delay, 0.0f, 10000.0f);
} 

float Particles::getDelay()
{
  return propagationDelay;
}

void  Particles::setMaxSpreadOutAngle(float angle)
{
  maximumAngle = clamp(angle, 0.0f, PI);
}

float Particles::getMaxSpreadOutAngle()
{
  return maximumAngle;
}

void  Particles::activeFading(bool fadeArg)
{
  fade = fadeArg;
}

bool  Particles::fadingActive()
{
  return fade;
}

void  Particles::activateSpinning(bool active)
{
  spin = active;
}

bool  Particles::spinningActive()
{
  return spin;
}

void  Particles::setStartSize(float size)
{
  startSize = clamp(size, 0.01f, 10000.0f);
}

float Particles::getStartSize()
{
  return startSize;
}

void  Particles::setEndSize(float size)
{
  endSize = clamp(size, 0.01f, 10000.0f);
}

float Particles::getEndSize()
{
  return endSize;
}

void Particles::setCenter(float x, float y, float z)
{
  center.set(x, y, z);
}

void Particles::setCenter(Tuple3f &xyz)
{
  center = xyz;
}

void  Particles::setExternalForce(float x, float y, float z)
{
  externalForce.set(x, y, z);
}

void  Particles::setExternalForce(const Tuple3f &xyz)
{
  externalForce.set(xyz);
}

const Tuple3f &Particles::getExternalForce() const
{
  return  externalForce;
}

void  Particles::setColor(float r, float g, float b)
{
  color.set(clamp(r, 0.0f, 1.0f), clamp(g, 0.0f, 1.0f), clamp(b, 0.0f, 1.0f));
}

void  Particles::setColor(const Tuple3f &rgb)
{
  setColor(rgb.x, rgb.y, rgb.z);
}

const Tuple3f &Particles::getColor() const
{
  return color;
}

void  Particles::setTexture(const Texture & textureArg)
{
  texture = textureArg;
}

void  Particles::setTexture(const char * textureArg)
{
  texture.load2D(textureArg, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,
                 GL_LINEAR, GL_LINEAR, false);
}

const Texture &Particles::getTexture() const
{
  return texture;
}

void  Particles::setParticlesCount(int particlesCount_)
{
  if(initialized)
    Logger::writeErrorLog("Fire char already initialized");
  else
    particlesCount = clamp(particlesCount_, 1, abs(particlesCount_));
}

int   Particles::getParticlesCount()
{
  return particlesCount;
}

void  Particles::setAcceleration(float acceleration_)
{
  acceleration = acceleration_;
}

float Particles::getAcceleration()
{
  return acceleration;
}

void  Particles::setRange(float range_)
{
  range = range_;
}

float Particles::getRange()
{
  return range;
}

void Particles::setBlendAttributes(const BlendAttributes &attri)
{
  blendAttr = attri;
}

BlendAttributes &Particles::getBlendAttributes()
{
  return blendAttr;
}

void Particles::setAlphaAttributes(const AlphaAttributes &attri)
{
  alphaAttr = attri;
}
 
AlphaAttributes &Particles::getAlphaAttributes()
{
  return alphaAttr;
} 

const Spline   & Particles::getColorSpline() const { return colorSpline; }
const Spline   & Particles::getPathSpline()  const { return pathSpline;  }
const Tuple3f  & Particles::getCenter()      const { return center;      }

Particles::~Particles()
{
  if(!initialized)
   return;

  _aligned_free(spinDirections);
  _aligned_free(lifeCycles);
  _aligned_free(sizes);
  _aligned_free(ages);
 
  _aligned_free(initialCenters);
  _aligned_free(directions);
  _aligned_free(texCoords);
  _aligned_free(centers);
  _aligned_free(coords);
  _aligned_free(colors);
}



