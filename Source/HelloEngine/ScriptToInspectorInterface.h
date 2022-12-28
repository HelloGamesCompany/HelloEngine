#pragma once
#include "Globals.h"
#include <string>
#include <vector>

#ifndef HELLO_ENGINE_EXPORTS
#include "json.hpp"
using json = nlohmann::json;
#endif
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
#ifndef HELLO_ENGINE_EXPORTS
	virtual void OnSerialize(json& j) = 0;
	virtual void OnDeserialize(json& j) = 0;
#endif
};

class DragField : public ScriptInspectorField
{
	void OnEditor() override;
#ifndef HELLO_ENGINE_EXPORTS
	void OnSerialize(json& j) override;
	void OnDeserialize(json& j) override;
#endif
};

class TO_API ScriptToInspectorInterface
{
public:
	virtual void AddDragFloat(std::string name, float* value) = 0;

protected:
	std::vector<ScriptInspectorField*> inspectorFields;
};
