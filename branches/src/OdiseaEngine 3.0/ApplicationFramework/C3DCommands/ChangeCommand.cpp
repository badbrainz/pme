#include "ChangeCommand.h"
#include "../C3DManagers/SceneManager.h"

void ChangeCommand::Execute()
{
	SceneManager::ChangeScene(m_Target);
}