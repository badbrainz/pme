#include "TextureNode.h"
#include "C3DVisitors/TileGraphVisitor.h"
#include <iostream>

TextureNode::TextureNode()
{
}

TextureNode::TextureNode(const TextureNode &copy)
{
  textureID = copy.textureID;
}

TextureNode& TextureNode::operator = (const TextureNode &right)
{
  textureID = right.textureID;
  return *this;
}

bool TextureNode::operator == (const TextureNode &right)
{
  return (textureID == right.textureID);
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