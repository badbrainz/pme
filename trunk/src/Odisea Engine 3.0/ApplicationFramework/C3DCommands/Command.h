#ifndef C3DCommand
#define C3DCommand

#include "Tools/Logger.h"

class XMLElement;

class Command
{
	public:
		Command(){}
		virtual ~Command(){}
	
	public:
    bool 					loadXMLSettings(XMLElement *element);
		
		void 					SetSource(const String& source);
		void 					SetTarget(const String& target);
		const String& GetSource();
		const String& GetTarget();
	
	public:
		virtual void 	Execute();
	
	protected:
		String m_Target;
		String m_Source;
};

#endif