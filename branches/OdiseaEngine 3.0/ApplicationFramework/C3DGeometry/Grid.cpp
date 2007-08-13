#include "Grid.h"

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

Grid::Grid()
{
  gridSize  = 8.0f;
  stepSize  = 1.0f;
  majorSize = 4.0f;
  yOffset   = 0.0f;
}

void Grid::Draw(void)
{
  glBegin(GL_LINES);
  DrawMinorGridLines();
  if(majorSize > 0.0f && majorSize <= gridSize)
    DrawMajorGridLines();
  glEnd();
}

void Grid::DrawMinorGridLines(void)
{
  glColor3ub(92, 80, 56);
  for(float f = -gridSize; f <= gridSize; f+= stepSize )
  {
    glVertex3f(f, yOffset, -gridSize);
    glVertex3f(f, yOffset, gridSize);
  }
  for(float g = -gridSize; g <= gridSize; g+= stepSize )
  {
    glVertex3f(gridSize, yOffset, g);
    glVertex3f(-gridSize, yOffset, g);
  }
}

void Grid::DrawMajorGridLines(void)
{
  glColor3ub(255, 162, 0);
  for(float f = -gridSize; f <= gridSize; f+= stepSize * majorSize )
  {
    glVertex3f(f, yOffset, -gridSize);
    glVertex3f(f, yOffset, gridSize);
  }
  for(float g = -gridSize; g <= gridSize; g+= stepSize * majorSize )
  {
    glVertex3f(gridSize, yOffset, g);
    glVertex3f(-gridSize, yOffset, g);
  }
}

void Grid::SetOffsetY(float value)
{
  yOffset = value;
}

void Grid::Setup(float step, float size, float major)
{
  gridSize  = size;
  stepSize  = step;
  majorSize = major;
}
