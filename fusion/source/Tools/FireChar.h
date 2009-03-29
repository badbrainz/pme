#ifndef FIRECHAR
#define FIRECHAR

#include "../Appearance/Texture.h"

#define FIRE_CHAR_ROWS 12
#define FIRE_CHAR_COLUMNS 7
#define TOTAL_FIRE_CHAR_SIZE FIRE_CHAR_ROWS*FIRE_CHAR_COLUMNS

class XMLElement;

typedef Array <Tuple2f> CharDots;

class FireChar
{
    static Matrix4f modelview;
    static Tuple3f  right;
    static Tuple3f  up;
    
    CharDots charLocations;
    
    Texture texture;
    
    Tuple4f *colors;
    
    Tuple3f color;
    Tuple3f *directions, *texCoords, *centers, *coords, *speeds;
    
    Tuple2f dotIntervals;
    bool initialized, spin;
    
    int particlesCount, activeParticlesCount, previousCount, particlesPerDot;
    int *indices;
    
    float maximumAngle, minimumLifeCycle;
    float startSize, endSize;
    float speed, propagationDelay, elapsedTime, timer;
    float width, height;
    float *spinDirections, *lifeCycles, *sizes, *ages;
    
    char keyChar;
    
  public:
    FireChar(int   particlesPerDot =       5,
             float startSize       =    1.0f,
             float endSize         =    1.0f,
             float speed           =   20.0f,
             float delay           =   0.25f,
             float minimumLifeT    =    2.0f,
             float maximumAngle    =   0.20f,
             bool  textureSpin  = false);
             
    ~FireChar();
    
  public:
  
    bool  loadXMLSettings(const char* path);
    bool  loadXMLSettings(XMLElement *particlesNode);
    
    bool  initialize();
    
    bool  updateAndCheckState(float tick, int index);
    void  updateLogic(float);
    void  draw();
    
    void  setColor(float r, float g, float b);
    void  setColor(const Tuple3f &rgb);
    const Tuple3f &getColor() const;
    
    void  setTexture(const Texture& texture);
    void  setTexture(const char* texture);
    const Texture & getTexture() const;
    
    bool  setKeyChar(XMLElement *cridChar);
    char  getKeyChar();
    
    void  setParticlesPerDotCount(int particlesPerDot);
    int   getParticlesPerDotCount();
    int   getParticlesCount();
    
    void  setSpeed(float speed);
    float getSpeed();
    
    void  setMinimumLifeCycle(float cycle);
    float getMinimumLifeCycle();
    
    void  setDelay(float delay);
    float getDelay();
    
    void  setMaxSpreadOutAngle(float angle);
    float getMaxSpreadOutAngle();
    
    void  activateSpinning(bool active);
    bool  spinningActive();
    
    void  setStartSize(float size);
    float getStartSize();
    
    void  setEndSize(float size);
    float getEndSize();
    
    float getHeight();
    float getWidth();
    
    void  setDotIntervals(const Tuple2f &xy);
    void  setDotIntervals(float x, float y);
    const Tuple2f &getDotIntervals() const;
    
    const CharDots *getCharLocations() const;
    
  protected:
    void updateLocations(int index);
    void generateSpeed(Tuple3f &speed);
    void resetParticle(int index);
    int  getSourceIndex(int particleIndex);
};

#endif
