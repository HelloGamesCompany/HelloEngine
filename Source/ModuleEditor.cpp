#include "ModuleEditor.h"
#include "Application.h"
#include "QuickSave.h"

ModuleEditor::ModuleEditor(bool enabled) :Module(enabled)
{
}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Start()
{
    QuickSave::SetInt("Value2", 2);
    QuickSave::SetInt("Value3", 3);
    QuickSave::SetInt("Value4", 4);

    int value1 = QuickSave::GetInt("Value1", -1);
    return true;
}

UpdateStatus ModuleEditor::PreUpdate()
{
    return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleEditor::Update()
{
    return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleEditor::PostUpdate()
{
    return UpdateStatus::UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
    return true;
}
