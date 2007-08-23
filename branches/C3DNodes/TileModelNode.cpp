#include "TileModelNode.h"
#include "C3DVisitors/TileGraphVisitor.h"
#include <iostream>

TileModelNode::TileModelNode()
{
}

TileModelNode::TileModelNode(const TileModelNode &copy)
{
  m_pController = copy.m_pController;
}

TileModelNode& TileModelNode::operator = (const TileModelNode &right)
{
  m_pController = right.m_pController;
  return *this;
}

bool TileModelNode::operator == (const TileModelNode &right)
{
  return (m_pController == right.m_pController);
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