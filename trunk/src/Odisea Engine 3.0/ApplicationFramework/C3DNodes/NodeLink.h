#ifndef NODELINK
#define NODELINK

class Node;

class NodeLink
{
	public:
		NodeLink();
	
	public:
		void				    SetParentNode(Node *node);
		void				    SetChildNode(Node *node);
		Node*           GetParentNode();
		Node*           GetChildNode();
		
		void				    SetPreviousLink(NodeLink *link);
		void				    SetNextLink(NodeLink *link);
		NodeLink*       GetPreviousLink();
		NodeLink*       GetNextLink();
	
	private:
		Node          *m_pParentNode,
							    *m_pChilNode;
		
		NodeLink      *m_pPreviousLink,
							    *m_pNextLink;
};

#endif