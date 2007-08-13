#include "NodeFactory.h"
#include "../RenderGraph/Nodes/Node.h"
#include "../RenderGraph/Nodes/TextureNode.h"

NodeResource NodeFactory::m_ManagedResources;

NodeResource::NodeResource() : m_Resources(100), m_Cursor(0)
{}

void NodeResource::AddResource(Node* node)
{
	m_Resources[m_Cursor++].Reset(node);
}

TextureNode* NodeFactory::CreateTextureNode()
{
	TextureNode* node = new TextureNode;
	m_ManagedResources.AddResource(node);
	return node;
}
