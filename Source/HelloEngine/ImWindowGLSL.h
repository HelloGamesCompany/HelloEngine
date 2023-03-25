#pragma once
#include "ImWindow.h"

#include "TextEditor.h"

class ResourceShader;

class ImWindowGLSL : public ImWindow
{
public:
	ImWindowGLSL();
	~ImWindowGLSL();

	void Update() override;

	void SetShader(int UID);

private:
	void MenuBar();
	void SaveAction();

	TextEditor editor;
	TextEditor::LanguageDefinition lang;

	ResourceShader* shader = nullptr;

	std::string lastSave;
	bool _edited = false;
	bool _compilationError = false;
};

