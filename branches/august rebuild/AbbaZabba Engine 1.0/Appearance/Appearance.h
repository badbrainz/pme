/*
 *Author: Abdul Bezrati
 *Email : abezrati@hotmail.com
 */

#ifndef APPEARANCE_H
#define APPEARANCE_H
 
#include "../Shaders/Shaders.h"
#include "../Math/MathUtils.h"
#include "BlendAttributes.h"
#include "AlphaAttributes.h"
#include "Material.h"
#include "Texture.h"

class Appearance : public IOXMLObject
{

  private:
    BlendAttributes  blendAttr;
    AlphaAttributes  alphaAttr;
    Material        *material;
    Shaders          shaders;
    Texture          textures[MAX_TEX_UNITS];
    Tuple4f          color; 
    bool             shadersEnabled,
                     colorEnabled;
                       
  public:
    Appearance();
   ~Appearance();
    Appearance(const Appearance&);
  	Appearance &operator=(const Appearance&);

    void destroy();

    virtual bool loadXMLSettings(XMLElement *element);
    virtual bool exportXMLSettings(ofstream &xmlFile);

    void                   setBlendAttributes(const BlendAttributes &attri);
    BlendAttributes &getBlendAttributes();

    void                   setAlphaAttributes(const AlphaAttributes &attri);
    AlphaAttributes &getAlphaAttributes();

    void      setMaterial(const Material &material);
    Material *getMaterial();

    void     setTexture(int index, Texture &texture);
    Texture *getTexture(int index);

    void     setShaders(Shaders &shaders);
    Shaders &getShaders();

    void setTriangleWinding(GLuint w);
    void setDepthMask(GLuint m);

    void  setColor(const float* c);
    void  setColor(float r, float g, float b, float a);
    const Tuple4f &getColor() const;

    void enableShaders(bool );
    void enableColor(bool);

    bool shadersOn();
    bool colorOn();
};
#endif