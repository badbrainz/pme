#include "BlendAttributesNode.h"
#include "C3DVisitors/TileGraphVisitor.h"
#include <iostream>

#include <windows.h>
#include <gl/glu.h>
#include <gl/gl.h>

BlendAttributesNode::BlendAttributesNode()
{
}

void BlendAttributesNode::Accept(TileGraphVisitor* visitor)
{
  visitor->Visit(this);
}

void BlendAttributesNode::SetAttributes()
{
}

BlendAttributesNode::~BlendAttributesNode()
{
}