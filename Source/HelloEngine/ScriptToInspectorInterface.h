#pragma once
#include "Globals.h"
#include <string>
#include <vector>

#ifndef HELLO_ENGINE_EXPORTS
#include "json.hpp"
using json = nlohmann::json;
#endif

class ScriptInspectorField
{
public:
	void* value = nullptr;
	std::string valueName = "";
	virtual void OnEditor() = 0;
#ifndef HELLO_ENGINE_EXPORTS
	virtual void OnSerialize(json& j) = 0;
	virtual void OnDeserialize(json& j) = 0;
#endif
};

class DragFieldFloat : public ScriptInspectorField
{
	void OnEditor() override;
#ifndef HELLO_ENGINE_EXPORTS
	void OnSerialize(json& j) override;
	void OnDeserialize(json& j) override;
#endif
};

class DragFieldInt : public ScriptInspectorField
{
	void OnEditor() override;
#ifndef HELLO_ENGINE_EXPORTS
	void OnSerialize(json& j) override;
	void OnDeserialize(json& j) override;
#endif
};

class CheckBoxField : public ScriptInspectorField
{
	void OnEditor() override;
#ifndef HELLO_ENGINE_EXPORTS
	void OnSerialize(json& j) override;
	void OnDeserialize(json& j) override;
#endif
};

class InputBoxField : public ScriptInspectorField
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
	virtual void AddDragFloat(const std::string& name, float* value) = 0;
	virtual void AddDragInt(const std::string& name, int* value) = 0;
	virtual void AddCheckBox(const std::string& name, bool* value) = 0;
	virtual void AddInputBox(const std::string& name, std::string* value) = 0;

protected:
	std::vector<ScriptInspectorField*> inspectorFields;
};
