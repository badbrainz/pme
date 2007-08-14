#ifndef NODEITERATOR
#define NODEITERATOR

class GraphNode;

class NodeIterator
{
	public:
		NodeIterator();
		NodeIterator(GraphNode* node);
	
	public:
		NodeIterator&		operator ++ ();
		NodeIterator 	  operator ++ (int);
		
		NodeIterator& 	operator -- ();
		NodeIterator 	  operator -- (int);
		
		bool            operator == (NodeIterator& right);
		bool 						operator != (NodeIterator& right);
		bool 						End();
		
		GraphNode*      Current();
	
	protected:
		GraphNode 	*m_pNode;
};

#endif