#include "ModuleEditor.h"

ModuleEditor::ModuleEditor(bool enabled) :Module(enabled)
{
}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Start()
{
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
