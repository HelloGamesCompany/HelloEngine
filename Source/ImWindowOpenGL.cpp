#include "Headers.h"
#include "ImWindowOpenGL.h"
#include "ModuleRenderer3D.h"
#include "XMLNode.h"
#include "ModuleXML.h"

ImWindowOpenGL::ImWindowOpenGL()
{
	windowName = "OpenGL Configuration";

	isEnabled = true;

	moduleRenderer = Application::Instance()->renderer3D;

	// Get openGl node with module xml
	XMLNode openGlNode = Application::Instance()->xml->GetConfigXML().FindChildBreadth("openGL");

	// Init setting values with node context
	for (pugi::xml_node n = openGlNode.node.first_child(); !n.empty(); n = n.next_sibling())
	{
		n.name();
		configs.insert(
			std::make_pair(	
				n.name(), // map key -> conifg name
				std::make_pair(	// map value -> pair<value,tag>
					n.attribute("value").as_bool(false),	// config value
					n.attribute("tag").as_int(0))));	// config tag
	}
}

ImWindowOpenGL::~ImWindowOpenGL()
{
	// Save values to the xml file
	XMLNode openGlNode = Application::Instance()->xml->GetConfigXML().FindChildBreadth("openGL");

	for (pugi::xml_node n = openGlNode.node.first_child(); !n.empty(); n = n.next_sibling())
		n.attribute("value").set_value(configs[n.name()].first);

	openGlNode.Save();
}

void ImWindowOpenGL::Update()
{
	if(ImGui::Begin(windowName.c_str(), &isEnabled))
	{
		// iter.first = config name,
		// iter.second.first = conifg value,
		// iter.second.second = config openGL tag
		for (auto& iter : configs)
		{
			ImGui::Checkbox(iter.first.c_str(), &iter.second.first);
			if (iter.first == "wireframe") moduleRenderer->ToggleOpenGLWireframe(iter.second.first);
			else moduleRenderer->ToggleOpenGLSystem(iter.second.first, iter.second.second);
		}
	}
	ImGui::End();
}