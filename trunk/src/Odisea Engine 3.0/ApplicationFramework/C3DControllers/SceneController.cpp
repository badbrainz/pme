#include "SceneController.h"
#include "Parsers/XMLParser.h"

#include "../C3DCommands/Command.h"
#include "../C3DCommands/SwitchCommand.h"
#include "../C3DCommands/ChangeCommand.h"
#include "../C3DCommands/DeactivateCommand.h"

SceneController::SceneController()
{}

bool SceneController::loadXMLSettings(XMLElement *element)
{
  XMLElement  *child		= 0,
              *subChild = 0;
	
	for (size_t i = 0; i < element->getChildrenCount(); i++)
	{
		if (!(child = element->getChild(i)))
      continue;
		
		if (child->getName() == "Command")
		if (subChild = child->getChildByName("type"))
		{
			const String& type = subChild->getValuec();
			
			if (type == "switch")
			{
				Command *command = new SwitchCommand;
				command->loadXMLSettings(child);
				AddCommand(command);
				continue;
			}
			
			if (type == "change")
			{
				Command *command = new ChangeCommand;
				command->loadXMLSettings(child);
				AddCommand(command);
				continue;
			}
			
			if (type == "deactivate")
			{
				Command *command = new DeactivateCommand;
				command->loadXMLSettings(child);
				AddCommand(command);
				continue;
			}
		}
	}

  return true;
}

void SceneController::AddCommand(Command* command)
{
	m_Commands[command->GetSource()] = command;
}

void SceneController::Execute(const String& callbackString)
{
	CommandMap::const_iterator iter = m_Commands.find(callbackString);
	if (iter != m_Commands.end())
		iter->second->Execute();
}

SceneController::~SceneController()
{
	CommandMap::iterator iter = m_Commands.begin();
	for (iter; iter != m_Commands.end(); iter++)
		if (iter->second) delete iter->second;
}