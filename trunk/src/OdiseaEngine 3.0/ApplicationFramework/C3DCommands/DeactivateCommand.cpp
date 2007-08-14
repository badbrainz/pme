#include "DeactivateCommand.h"
#include "../C3DManagers/SceneManager.h"

void DeactivateCommand::Execute()
{
	SceneManager::DeactivateCurrentStage();
}