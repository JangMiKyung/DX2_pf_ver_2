#include "stdafx.h"
#include "Command.h"
#include "State.h"
#include "Unit.h"

#include "cTransform.h"
#include "Unit.h"


void WalkCommand::execute(Unit& gameActor, float timeDelta)
{
	
		if (KEY_MGR->IsStayDown(VK_SHIFT))
			gameActor.StatePlayChange("RUN_FORWARD", 0.4f);
		else if (!KEY_MGR->IsStayDown(VK_SHIFT))
			gameActor.StatePlayChange("WALK_FORWARD", 0.4f);

	gameActor.GetState()->Move(timeDelta);
}

void SitCommand::execute(Unit& gameActor, float timeDelta)
{
	gameActor.StatePlayChange("CROUCH_IDLE", 0.5f);
	
}

void RunCommand::execute( Unit& gameActor, float timeDelta)
{
	gameActor.StateOneShotHoldChange("RUN_FORWARD", 1.0f);
}

void NULLCommand::execute(Unit& gameActor, float timeDelta)
{
	if (!KEY_MGR->IsStayDown(VK_CONTROL))
	gameActor.StatePlayChange("STAND_IDLE", 0.3f);

}

void UpStandCommand::execute(Unit & gameActor, float timeDelta)
{
	gameActor.StateOneShotChange("COURACH_TO_STAND", 0.5f);

}

InputHandler::InputHandler()
{
	ButtonCtrl = new SitCommand;
	ButtonWASD = new WalkCommand;
	ButtonCtrlUo = new UpStandCommand;
	ButtonShift = new RunCommand;
	ButtonNULL = new NULLCommand;
}

Command * InputHandler::HandleInput()
{
	if ( (KEY_MGR->IsStayDown('W') || KEY_MGR->IsStayDown('S') || KEY_MGR->IsStayDown('D') || KEY_MGR->IsStayDown('A') )) 
		return ButtonWASD;
	if (KEY_MGR->IsStayDown(VK_CONTROL))
		return ButtonCtrl;

	return ButtonNULL;
}


