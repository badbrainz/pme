#ifndef SHAPE_H
#define SHAPE_H

#include "../Appearance/Appearance.h"
#include "../Geometry/ShadowInfo.h"
#include "../Geometry/Geometry.h"

class TransformGroup;
class Shape
{
  public:
    Shape(Geometry* geom = NULL, Appearance *app = NULL);
    Shape(const Shape&);
   ~Shape();
    Shape &operator=(const Shape&);

    bool loadXMLSettings(XMLElement *element, const char* shapePath);

    bool exportXMLSettings(ofstream &xmlFile  );
    bool exportXMLSettings(const char* xmlPath);

    void            setParent(TransformGroup *);
    TransformGroup *getParent();

    void initialize();

    void        setAppearance(Appearance *app);
    Appearance *getAppearance();

    void       setGeometry(Geometry *geom);
    Geometry  *getGeometry();

    void setVisible(bool vi);
    bool isVisible();

    void doShadowPass(Tuple4f &lightPos, bool, int renderingPath);
    void renderSilhouette(Tuple4f &viewPosition);

    void compile(bool);
    int  draw(int format);

    int  getPickingIndex();
  private:
    void drawVBOs(int rendering);
    void drawRegular(int rendering);

    bool isTextureActive(int format, int index);
    void enableTextureUnits(int enabledTextures);
    void disableTexureUnits(int enabledTextures);

    TransformGroup *parent;
    Appearance*     appearance;
    Geometry*       geometry;
    GLuint          pickingIndex;
    bool            visible;
};
#endif