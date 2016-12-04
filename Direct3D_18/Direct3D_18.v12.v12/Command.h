#pragma once

class Unit;



class Command
{

public:
	virtual ~Command() {};
	virtual void execute(Unit& gameActor, float timeDelta) = 0;
};

class WalkCommand : public Command
{
public:
	WalkCommand() {};
	virtual void execute(Unit& gameActor, float timeDelta);

};

class SitCommand : public Command
{
public:
	SitCommand() {};
	virtual void execute(Unit& gameActor, float timeDelta);

};

class RunCommand : public Command
{
public:
	RunCommand() {};
	virtual void execute(Unit& gameActor, float timeDelta);

};


class UpStandCommand : public Command
{
public:
	UpStandCommand() {};
	virtual void execute(Unit& gameActor, float timeDelta);

};

class NULLCommand : public Command
{
public:
	NULLCommand() {};
	virtual void execute(Unit& gameActor, float timeDelta);

};

class InputHandler
{
	Command* ButtonWASD;
	Command* ButtonCtrl;
	Command* ButtonNULL;
	Command* ButtonShift;
	Command* ButtonCtrlUo;

public:
	InputHandler();
	Command* HandleInput();

};




