#include "TileModelNode.h"
#include "C3DVisitors/TileGraphVisitor.h"
#include <iostream>

TileModelNode::TileModelNode()
{
}

void TileModelNode::Accept(TileGraphVisitor* visitor)
{
  visitor->Visit(this);
}

void TileModelNode::SetController(TileModelController *controller)
{
  m_pController = controller;
}

TileModelController* TileModelNode::GetController()
{
  return m_pController;
}

TileModelNode::~TileModelNode()
{
}