#pragma once
#include "Globals.h"
#include <string>
#include <vector>

enum class FieldType
{
	DRAG,
	CHECK_BOX,
	INPUT_BOX
};

class ScriptInspectorField
{
public:
	void* value = nullptr;
	std::string valueName = "";
	FieldType type;
	virtual void OnEditor() = 0;
};

class DragField : public ScriptInspectorField
{
	void OnEditor() override;
};

class TO_API ScriptToInspectorInterface
{
public:
	virtual void AddDragFloat(std::string name, float* value) = 0;

protected:
	std::vector<ScriptInspectorField*> inspectorFields;
};
