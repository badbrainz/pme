#include "TextureNode.h"
#include "C3DVisitors/TileGraphVisitor.h"
#include <iostream>

TextureNode::TextureNode()
{
}

void TextureNode::Accept(TileGraphVisitor* visitor)
{
  visitor->Visit(this);
}

void TextureNode::SetTextureID(unsigned int id)
{
  textureID = id;
}

unsigned int TextureNode::GetTextureID()
{
  return textureID;
}

TextureNode::~TextureNode()
{
}