#ifndef GRAPHNODELINK
#define GRAPHNODELINK

class GraphNode;

class GraphNodeLink
{
	public:
		GraphNodeLink();
	
	public:
		void				    SetParentNode(GraphNode *node);
		void				    SetChildNode(GraphNode *node);
		GraphNode*      GetParentNode();
		GraphNode*      GetChildNode();
		
		void				    SetPreviousLink(GraphNodeLink *link);
		void				    SetNextLink(GraphNodeLink *link);
		GraphNodeLink*  GetPreviousLink();
		GraphNodeLink*  GetNextLink();
	
	private:
		GraphNode     *m_pParentNode,
							    *m_pChilNode;
		
		GraphNodeLink *m_pPreviousLink,
							    *m_pNextLink;
};

#endif