#include "Headers.h"
#include "ImWindowAbout.h"

ImWindowAbout::ImWindowAbout()
{
	windowName = "About";
	isEnabled = true;

	// Get Versions -------------------------------
	SDL_version v; 
	SDL_GetVersion(&v);
	sdlVersion = std::to_string(v.major) + "." + std::to_string(v.minor) + "." + std::to_string(v.patch);
	glewVersion = (const char*)glewGetString(GLEW_VERSION);
	imGUIVersion = ImGui::GetVersion();
	jsonVersion = "3.12.2";
	mathGeoLibVersion = "1.5";
	pugiXmlVersion = "1.6";
}

void ImWindowAbout::Update()
{

	if(ImGui::Begin(windowName.c_str(), &isEnabled))
	{
		ImGui::TextWrapped("HelloEngine %s", Application::Instance()->GetEngineVersion().c_str());
		ImGui::TextWrapped("by Zhida Chen & Adria Sellares");

		ImGui::NewLine();

		ImGui::TextWrapped("Libraries used: ");
		ImGui::BulletText("SDL %s", sdlVersion.c_str());
		ImGui::BulletText("Glew %s", glewVersion.c_str());
		ImGui::BulletText("ImGUI %s", imGUIVersion.c_str());
		ImGui::BulletText("JSON by nlhomann %s", jsonVersion.c_str());
		ImGui::BulletText("MathGeoLib %s", mathGeoLibVersion.c_str());
		ImGui::BulletText("PugiXML %s", pugiXmlVersion.c_str());

		ImGui::NewLine();

		ImGui::TextWrapped("License information: ");
		ImGui::NewLine();

		ImGui::TextWrapped("MIT License");
		ImGui::NewLine();

		ImGui::TextWrapped("Copyright (c) 2022 HelloGamesCompany");
		ImGui::NewLine();

		ImGui::TextWrapped("Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the \"Software\") to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and /or sell to use, copy, modify, merge, publish, distribute, sublicense, and /or sellcopies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions : ");
		ImGui::NewLine();
		ImGui::TextWrapped("The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.");
		ImGui::NewLine();
		ImGui::TextWrapped("THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.");

	}

	ImGui::End();
}
