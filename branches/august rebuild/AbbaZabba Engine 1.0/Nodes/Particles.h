#ifndef PARTICLES_H
#define PARTICLES_H

#include "../Appearance/BlendAttributes.h"
#include "../Appearance/AlphaAttributes.h"
#include "../Appearance/Texture.h"
#include "../Geometry/DistanceObject.h"
#include "../Tools/Spline.h"

typedef vector<Tuple4f>         CollisionSpheres;
typedef DistanceObject<int>     DistanceObjecti;
typedef vector<DistanceObjecti> DistanceBufferi;

class Particles
{
  private:
    bool       sort(int order, float x, float y, float z);

    static     Matrix4f modelview;

    static     Tuple3f  right,
                        up;

    BlendAttributes blendAttr;
    AlphaAttributes alphaAttr;

    Texture    texture;

    Spline     pathSpline, 
               colorSpline;

    Tuple3f   *colors,
               color;
 
    Tuple3f   *initialCenters,
               externalForce,
              *directions,
              *texCoords,
              *centers,
              *coords,
               center;

    float      propagationDelay,
               minimumLifeCycle, 
              *spinDirections,  
               maximumAngle,               
               acceleration,
               elapsedTime,
              *lifeCycles,
               startSize, 
               endSize,
               timer,
               speed,
               range,
              *sizes,
              *ages;

    bool       initialized,
               collisionOn,
               fade,
               spin;

    int        activeParticlesCount,
               particlesCount,  
               previousCount,
              *sortedIndices;

    CollisionSpheres  collisionSpheres;  
    DistanceBufferi   distanceProcessor;

  public:
    Particles(int   numParticles =     50,
              float startSize    =  24.0f,
              float endSize      =  24.0f,
              float speed        =  75.0f,
              float delay        =   2.0f,
              float minimumLifeT =   2.0f,
              float maximumAngle =  0.20f,
              float acceleration =   1.0f,
              bool  textureSpin  =  false,
              bool  fade         =  true);
   ~Particles();

    bool  loadXMLSettings(const char*xmlPath);
    bool  loadXMLSettings(XMLElement *particlesNode);

    void  initialize();
    void  getNewDirection(Tuple3f &speed);
    void  updateLocations(int index);
    void  resetParticle(int index);

    void  draw(int order = 0, float xCam = 0, float yCam = 0, float zCam = 0);
    void  draw(int order, const Tuple3f &cameraPosition);

    void  updateLogic(float);

    void  setParticlesCount(int particlesCount);
    int   getParticlesCount();

    void  setMinimumLifeCycle(float cycle);
    float getMinimumLifeCycle();

    void  setAcceleration(float acceleration);
    float getAcceleration();

    void  setDelay(float delay);
    float getDelay();

    void  setMaxSpreadOutAngle(float angle);
    float getMaxSpreadOutAngle();

    void  activateSpinning(bool active);
    bool  spinningActive();

    void  activeFading(bool fade);
    bool  fadingActive();

    void  setStartSize(float size);
    float getStartSize();

    void  setEndSize(float size);
    float getEndSize();
    
    void  setExternalForce(float x, float y, float z);
    void  setExternalForce(const Tuple3f &xyz);
    const Tuple3f &getExternalForce() const;

    void  setColor(float r, float g, float b);
    void  setColor(const Tuple3f &rgb);
    const Tuple3f &getColor() const;

    void  setTexture(const Texture & texture);
    void  setTexture(const char * texture);
    const Texture & getTexture() const;

    void  setCenter(float x, float y, float z);
    void  setCenter(Tuple3f &xyz);

    void  addCollisionSphere(const Tuple4f &sphere);

    bool  updateAndCheckState(float tick, int index);

    void  enableCollision(bool );
    bool  collisionEnabled();

    void                   setBlendAttributes(const BlendAttributes &attri);
    BlendAttributes &getBlendAttributes();

    void                   setAlphaAttributes(const AlphaAttributes &attri);
    AlphaAttributes &getAlphaAttributes();

    void  setRange(float range);
    float getRange();

    const Spline   &getColorSpline() const;
    const Spline   &getPathSpline()  const;
    const Tuple3f  &getCenter() const;
};

#endif