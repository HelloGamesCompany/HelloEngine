#include "Headers.h"
#include "ImWindowGLSL.h"

#include "ModuleResourceManager.h"

ImWindowGLSL::ImWindowGLSL()
{
	windowName = "GLSL Editor";

	isEnabled = false;

	lang = TextEditor::LanguageDefinition::GLSL();
	editor.SetLanguageDefinition(lang);
}

ImWindowGLSL::~ImWindowGLSL()
{

}

void ImWindowGLSL::Update()
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar;

	//Key handling
	if (editor.GetSaveRequest()) SaveAction();

	if (editor.IsTextChanged())
	{
		_edited = true;
	}
	if (_edited)
	{
		flags |= ImGuiWindowFlags_UnsavedDocument;
	}

	if (ImGui::Begin(windowName.c_str(), &isEnabled, flags))
	{
		MenuBar();

		std::string name = "";
		if (shader) name = shader->debugName;

		//Editor Render
		auto cpos = editor.GetCursorPosition();
		ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, editor.GetTotalLines(),
			editor.IsOverwrite() ? "Ovr" : "Ins",
			editor.CanUndo() ? "Und" : " ",
			editor.GetLanguageDefinition().mName.c_str(), name.c_str());

		//Compilation error feedback
		if (_compilationError)
		{
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "Compilation error: Check console!");
		}

		editor.Render("GLSL Editor");
	}
	ImGui::End();
}

void ImWindowGLSL::MenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save", "Ctrl-S"))
			{
				SaveAction();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			bool ro = editor.IsReadOnly();

			if (ImGui::MenuItem("Undo", "Ctrl-Z", nullptr, !ro && editor.CanUndo()))
				editor.Undo();
			if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && editor.CanRedo()))
				editor.Redo();

			ImGui::Separator();

			if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, editor.HasSelection()))
				editor.Copy();
			if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && editor.HasSelection()))
				editor.Cut();
			if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && editor.HasSelection()))
				editor.Delete();
			if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
				editor.Paste();

			ImGui::Separator();

			if (ImGui::MenuItem("Select all", nullptr, nullptr))
				editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(editor.GetTotalLines(), 0));

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Dark palette"))
				editor.SetPalette(TextEditor::GetDarkPalette());
			if (ImGui::MenuItem("Light palette"))
				editor.SetPalette(TextEditor::GetLightPalette());
			if (ImGui::MenuItem("Retro blue palette"))
				editor.SetPalette(TextEditor::GetRetroBluePalette());
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}

void ImWindowGLSL::SaveAction()
{
	_edited = false;

	shader->Recompile(editor.GetText());
	//LibraryManager::SaveJSON(currentResource->GetAssetsFile(), editor.GetText());//Save .shader
	//editorData.compilationError = ShaderManager::ImportToLibrary(currentResource);//Compile
}

void ImWindowGLSL::SetShader(int UID)
{
	isEnabled = true;
	//Clean previous data
	if (shader != nullptr)
	{
		this->shader->_onEditor = false;
	}

	this->shader = (ResourceShader*)ModuleResourceManager::resources[UID];

	if (this->shader == nullptr) return;

	this->shader->_onEditor = true;

	//Sets text on editor
	char* text = nullptr;
	ModuleFiles::S_Load(shader->resourcePath, &text);

	//editorData.lastSave = text;
	editor.SetText(text);

	RELEASE(text);
}