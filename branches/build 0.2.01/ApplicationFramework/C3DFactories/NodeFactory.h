#ifndef NODEFACTORY
#define NODEFACTORY

#include "../Pointers/VectorMemberPointer.h"

class Node;
class TextureNode;

class NodeResource
{
	public:
		NodeResource();
		
		void AddResource(Node* node);
	
	//private:
		VectorMemberPointer <Node> m_Resources;
		int m_Cursor;
};

class NodeFactory
{
	public:
		static TextureNode* CreateTextureNode();
	
	private:
		static NodeResource m_ManagedResources;
};

#endif