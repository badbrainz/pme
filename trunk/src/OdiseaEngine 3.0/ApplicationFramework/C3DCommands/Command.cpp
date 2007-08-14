#include "Command.h"
#include "Parsers/XMLParser.h"

bool Command::loadXMLSettings(XMLElement *element)
{
	XMLElement  *subChild = 0;
	
	if (subChild = element->getChildByName("source"))
		SetSource(subChild->getValuec());
	
	if (subChild = element->getChildByName("target"))
		SetTarget(subChild->getValuec());
	
	return true;
}

void Command::SetSource(const String& source)
{
	m_Source = source;
}

void Command::SetTarget(const String& target)
{
	m_Target = target;
}

const String& Command::GetSource()
{
	return m_Source;
}

const String& Command::GetTarget()
{
	return m_Target;
}

void Command::Execute()
{
}