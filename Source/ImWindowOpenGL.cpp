#include "Headers.h"
#include "ImWindowOpenGL.h"
#include "ModuleRenderer3D.h"

ImWindowOpenGL::ImWindowOpenGL()
{
	windowName = "OpenGL Configuration";
	isEnabled = true;

	moduleRenderer = Application::Instance()->renderer3D;
}

void ImWindowOpenGL::Update()
{
	if(ImGui::Begin(windowName.c_str(), &isEnabled))
	{
		ImGui::Checkbox("Depth Test", &depthTest);
		ImGui::Checkbox("Cull Face", &cullFace);
		ImGui::Checkbox("Lighting", &lighting);
		ImGui::Checkbox("Color Material", &colorMaterial);
		ImGui::Checkbox("Texture 2D", &texture2D);
		ImGui::Checkbox("Blend", &blend);

		// Call glEnable/disable for every checkbox. If the system is already on the given state, it ignores the call.
		moduleRenderer->ToggleOpenGLSystem(depthTest, GL_DEPTH);
		moduleRenderer->ToggleOpenGLSystem(cullFace, GL_CULL_FACE);
		moduleRenderer->ToggleOpenGLSystem(lighting, GL_LIGHTING);
		moduleRenderer->ToggleOpenGLSystem(colorMaterial, GL_COLOR_MATERIAL);
		moduleRenderer->ToggleOpenGLSystem(texture2D, GL_TEXTURE_2D);
		moduleRenderer->ToggleOpenGLSystem(blend, GL_BLEND);

	}
	ImGui::End();

}
