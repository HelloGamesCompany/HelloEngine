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

	// Init setting values with xml
	XMLNode openGlNode = Application::Instance()->xml->GetConfigXML().FindChildBreadth("openGL");

	//for (pugi::xml_node n = openGlNode.node.first_child(); !n.empty(); n = n.next_sibling())
	//{
	//	n.name();
	//	configs.insert(std::make_pair(n.name(), n.attribute("value").as_bool(false)));
	//}

	depthTest = openGlNode.node.child("depthTest").attribute("value").as_bool(false);
	cullFace = openGlNode.node.child("cullFace").attribute("value").as_bool(false);
	lighting = openGlNode.node.child("lighting").attribute("value").as_bool(false);
	colorMaterial = openGlNode.node.child("colorMaterial").attribute("value").as_bool(false);
	texture2D = openGlNode.node.child("texture2D").attribute("value").as_bool(false);
	blend = openGlNode.node.child("blend").attribute("value").as_bool(false);
	wireframe = openGlNode.node.child("wireframe").attribute("value").as_bool(false);
}

ImWindowOpenGL::~ImWindowOpenGL()
{
	// Save values to the xml file
	XMLNode openGlNode = Application::Instance()->xml->GetConfigXML().FindChildBreadth("openGL");

	//for (pugi::xml_node n = openGlNode.node.first_child(); !n.empty(); n = n.next_sibling())
	//{
	//	n.attribute("value").set_value(configs[n.name()]);
	//}

	openGlNode.node.child("depthTest").attribute("value").set_value(depthTest);
	openGlNode.node.child("cullFace").attribute("value").set_value(cullFace);
	openGlNode.node.child("lighting").attribute("value").set_value(lighting);
	openGlNode.node.child("colorMaterial").attribute("value").set_value(colorMaterial);
	openGlNode.node.child("texture2D").attribute("value").set_value(texture2D);
	openGlNode.node.child("blend").attribute("value").set_value(blend);
	openGlNode.node.child("wireframe").attribute("value").set_value(wireframe);

	openGlNode.Save();
}

void ImWindowOpenGL::Update()
{
	if(ImGui::Begin(windowName.c_str(), &isEnabled))
	{
		//for (auto& iter : configs)
		//{
		//	ImGui::Checkbox(iter.first.c_str(), &iter.second);
		//}

		ImGui::Checkbox("Depth Test", &depthTest);
		ImGui::Checkbox("Cull Face", &cullFace);
		ImGui::Checkbox("Lighting", &lighting);
		ImGui::Checkbox("Color Material", &colorMaterial);
		ImGui::Checkbox("Texture 2D", &texture2D);
		ImGui::Checkbox("Blend", &blend);
		ImGui::Checkbox("Wireframe", &wireframe);

		// Call glEnable/disable for every checkbox. If the system is already on the given state, it ignores the call.
		moduleRenderer->ToggleOpenGLSystem(depthTest, GL_DEPTH);
		moduleRenderer->ToggleOpenGLSystem(cullFace, GL_CULL_FACE);
		moduleRenderer->ToggleOpenGLSystem(lighting, GL_LIGHTING);
		moduleRenderer->ToggleOpenGLSystem(colorMaterial, GL_COLOR_MATERIAL);
		moduleRenderer->ToggleOpenGLSystem(texture2D, GL_TEXTURE_2D);
		moduleRenderer->ToggleOpenGLSystem(blend, GL_BLEND);
		moduleRenderer->ToggleOpenGLWireframe(wireframe);
	}
	ImGui::End();
}
