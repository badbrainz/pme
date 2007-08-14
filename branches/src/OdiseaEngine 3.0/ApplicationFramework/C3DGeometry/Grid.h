#ifndef GRID
#define GRID

class Grid
{
public:
   Grid();
  ~Grid(){}

  void Draw(void);
  void SetOffsetY(float value);
  void Setup(float step, float size, float major);

private:
  void DrawMinorGridLines(void);
  void DrawMajorGridLines(void);

  float gridSize,
        stepSize,
        majorSize,
        yOffset;
};

#endif