#include "TileGraphDeactivatorVisitor.h"
#include "../NodeIterator.h"

#include "../IndicesNode.h"
#include "../TextureNode.h"
#include "../TileModelNode.h"
#include "../TextureCoordsNode.h"
#include "../BlendAttributesNode.h"

#include <iostream>

void TileGraphDeactivatorVisitor::Visit(Node* node)
{
  NodeIterator iter(node->GetFirstChild());
  
  while (!iter.End())
  {
    iter.Current()->Accept(this);
    iter++;
  }
}

void TileGraphDeactivatorVisitor::Visit(IndicesNode* node)
{
  NodeIterator iter(node->GetFirstChild());
  
  while (!iter.End())
  {
    iter.Current()->Accept(this);
    iter++;
  }
}

void TileGraphDeactivatorVisitor::Visit(BlendAttributesNode* node)
{
  NodeIterator iter(node->GetFirstChild());
  
  while (!iter.End())
  {
    iter.Current()->Accept(this);
    iter++;
  }
}

void TileGraphDeactivatorVisitor::Visit(TextureNode* node)
{
  NodeIterator iter(node->GetFirstChild());
  
  while (!iter.End())
  {
    iter.Current()->Accept(this);
    iter++;
  }
}

void TileGraphDeactivatorVisitor::Visit(TextureCoordsNode* node)
{
  NodeIterator iter(node->GetFirstChild());
  
  while (!iter.End())
  {
    iter.Current()->Accept(this);
    iter++;
  }
}

void TileGraphDeactivatorVisitor::Visit(TileModelNode* node)
{
  NodeIterator iter(node->GetFirstChild());
}
