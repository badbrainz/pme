#include "TileGraphRendererVisitor.h"

#include "../NodeIterator.h"
#include "../../C3DControllers/TileModelController.h"

#include "../IndicesNode.h"
#include "../TextureNode.h"
#include "../TileModelNode.h"
#include "../TextureCoordsNode.h"
#include "../BlendAttributesNode.h"

#include "../../C3DParsers/SubTextureCoords.h"

#include <windows.h>
#include <gl/glu.h>
#include <gl/gl.h>

TileGraphRendererVisitor::TileGraphRendererVisitor()
{
  enableBlend = false;
}

void TileGraphRendererVisitor::EnableBlend(bool enable)
{
  enableBlend = enable;
}

void TileGraphRendererVisitor::Visit(GraphNode* node)
{
  NodeIterator iter(node->GetFirstChild());
  
  if (enableBlend)
  {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    while (!iter.End())
    {
      iter.Current()->Accept(this);
      iter++;
    }
    glDisable(GL_BLEND);
  }
  else
  {
    while (!iter.End())
    {
      iter.Current()->Accept(this);
      iter++;
    }
  }
}

void TileGraphRendererVisitor::Visit(IndicesNode* node)
{
  NodeIterator iter(node->GetFirstChild());
  
  m_pIndices = node->GetIndices();
  
  while (!iter.End())
  {
    iter.Current()->Accept(this);
    iter++;
  }
}

void TileGraphRendererVisitor::Visit(BlendAttributesNode* node)
{
  NodeIterator iter(node->GetFirstChild());
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  while (!iter.End())
  {
    iter.Current()->Accept(this);
    iter++;
  }
  
  glDisable(GL_BLEND);
}

void TileGraphRendererVisitor::Visit(TextureNode* node)
{
  NodeIterator iter(node->GetFirstChild());
  
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, node->GetTextureID());
  
  while (!iter.End())
  {
    iter.Current()->Accept(this);
    iter++;
  }
  
  glDisable(GL_TEXTURE_2D);
}

void TileGraphRendererVisitor::Visit(TextureCoordsNode* node)
{
  NodeIterator iter(node->GetFirstChild());
  
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  
  m_pCoords = node->GetTextureCoords();
  
  glTexCoordPointer(2, GL_FLOAT, 0, m_pCoords);
  
  while (!iter.End())
  {
    iter.Current()->Accept(this);
    iter++;
  }
  
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void TileGraphRendererVisitor::Visit(TileModelNode* node)
{
  TileModelController *controller = node->GetController();

  TransformTextureMatrix(controller->GetModelFlags());

  glVertexPointer(3, GL_FLOAT, 0, controller->GetVertices());
  glColorPointer(4, GL_UNSIGNED_BYTE, 0, controller->GetColors());
  glDrawElements(GL_TRIANGLE_FAN, 10, GL_UNSIGNED_INT, m_pIndices);
  
  UnTransformTextureMatrix();
}

void TileGraphRendererVisitor::TransformTextureMatrix(unsigned short flags)
{
  glMatrixMode(GL_TEXTURE);
  glPushMatrix();
  
  ///bad area!
  //*/
  if (flags & TileFlags::TEXTURE_MIRRORX)
  {
    glTranslatef(m_pCoords[4].x, m_pCoords[4].y, 0);
    glScalef(-1,1,1);
    glTranslatef(-m_pCoords[4].x, -m_pCoords[4].y, 0);
  }
  if (flags & TileFlags::TEXTURE_MIRRORY)
  {
    glTranslatef(m_pCoords[4].x, m_pCoords[4].y, 0);
    glScalef(1,-1,1);
    glTranslatef(-m_pCoords[4].x, -m_pCoords[4].y, 0);
  }
  //*/
  
  //*/
  if (flags & TileFlags::TEXTURE_ROTATE90)
  {
    glTranslatef(m_pCoords[4].x, m_pCoords[4].y, 0);
    glRotatef(90.0f,0,0,1);
    glTranslatef(-m_pCoords[4].x, -m_pCoords[4].y, 0);
  }
  else if (flags & TileFlags::TEXTURE_ROTATE180)
  {
    glTranslatef(m_pCoords[4].x, m_pCoords[4].y, 0);
    glRotatef(180.0f,0,0,1);
    glTranslatef(-m_pCoords[4].x, -m_pCoords[4].y, 0);
  }
  else if (flags & TileFlags::TEXTURE_ROTATE270)
  {
    glTranslatef(m_pCoords[4].x, m_pCoords[4].y, 0);
    glRotatef(270.0f,0,0,1);
    glTranslatef(-m_pCoords[4].x, -m_pCoords[4].y, 0);
  }
  //*/

  glMatrixMode(GL_MODELVIEW);
}

void TileGraphRendererVisitor::UnTransformTextureMatrix()
{
  glMatrixMode(GL_TEXTURE);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}
