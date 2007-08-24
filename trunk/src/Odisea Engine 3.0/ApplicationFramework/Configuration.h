#ifndef CONFIGURATION
#define CONFIGURATION

#include "Parsers/IOXMLObject.h"

class Configuration : public IOXMLObject
{
	public:
		 Configuration();
		~Configuration();
	
	public:
		virtual bool loadXMLSettings(XMLElement *element);
    virtual bool exportXMLSettings(ofstream &xmlFile){return true;}
	
	public:
		bool Initialize();
};

#endif