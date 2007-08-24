#include "SwitchCommand.h"
#include "../C3DManagers/SceneManager.h"

void SwitchCommand::Execute()
{
	SceneManager::SwitchScene(m_Target);
}
