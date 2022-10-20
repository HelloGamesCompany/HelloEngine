#pragma once
class Command
{
public:
	Command();

	virtual ~Command();

	virtual void Undo() = 0;

	virtual void Redo() = 0;
};