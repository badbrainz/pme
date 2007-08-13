#ifndef FACTORY
#define FACTORY

#include "Tools/String.h"
#include <map>

template <class T, class U> class Factory
{
	public:
		Factory(){}
		virtual ~Factory(){}
		
		void RegisterBuilder(const String &name, U builder)
		{
			BuilderList::const_iterator iter = m_BuilderList.find(name);
			
			if (iter != m_BuilderList.end())
				return;
			
			m_BuilderList[name] = builder;
		}
		
		T Create(const String &builderName)
		{
			BuilderList::const_iterator iter = m_BuilderList.find(name);
			
			if(iter == m_BuilderList.end())
				return 0;
			
			return iter->second->Build();
		}
	
	protected:
    typedef std::map <String, U> BuilderList;
		BuilderList m_BuilderList;
};

#endif