#ifndef SCENECONTROLLER
#define SCENECONTROLLER

#include "Tools/Logger.h"
#include <map>

class XMLElement;
class Command;

typedef std::map <String, Command*> CommandMap;

class SceneController
{
	public:
		 SceneController();
		~SceneController();
	
	public:
    bool loadXMLSettings(XMLElement *element);
		void Execute(const String& callbackString);
		void AddCommand(Command* command);
	
	private:
		CommandMap m_Commands;
};

#endif