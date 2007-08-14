#include "TextureCoordsNode.h"
#include "C3DVisitors/TileGraphVisitor.h"
#include <iostream>

TextureCoordsNode::TextureCoordsNode()
{
}

void TextureCoordsNode::Accept(TileGraphVisitor* visitor)
{
  visitor->Visit(this);
}

void TextureCoordsNode::SetTextureCoords(Tuple2f *coords)
{
  texCoords = coords;
}

Tuple2f* TextureCoordsNode::GetTextureCoords()
{
  return texCoords;
}

TextureCoordsNode::~TextureCoordsNode()
{
}