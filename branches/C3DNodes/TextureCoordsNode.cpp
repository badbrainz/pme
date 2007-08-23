#include "TextureCoordsNode.h"
#include "C3DVisitors/TileGraphVisitor.h"
#include <iostream>

TextureCoordsNode::TextureCoordsNode()
{
}

TextureCoordsNode::TextureCoordsNode(const TextureCoordsNode &copy)
{
  memcpy(texCoords, copy.texCoords, sizeof(Tuple2f)*9);
}

TextureCoordsNode& TextureCoordsNode::operator = (const TextureCoordsNode &right)
{
  texCoords = right.texCoords;
  return *this;
}

bool TextureCoordsNode::operator == (const TextureCoordsNode &right)
{
  return (texCoords == right.texCoords);
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