#include "Headers.h"
#include "TextRendererComponent.h"
#include "ModuleRenderer3D.h"
#include "RenderManager.h"

TextRendererComponent::TextRendererComponent(GameObject* go) : Component(go)
{
	_type = Component::Type::UI_TEXT;
	_needsTransformCallback = true;
	CreateText();
}

TextRendererComponent::~TextRendererComponent()
{
}

void TextRendererComponent::OnEditor()
{
	bool created = true;
	if (ImGui::CollapsingHeader("Text Renderer", &created, ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool auxiliaryBool = _isEnabled;
		if (ImGui::Checkbox("Active##textrenderer", &auxiliaryBool))
			auxiliaryBool ? Enable() : Disable();

		if (ImGui::ColorEdit4("Text color", &color.x))
		{
			ChangeColor(color);
		}
		if (ImGui::DragFloat("Scale", &scale, 0.1f, 0.0f))
		{
			ChangeScale(scale);
		}
		if (ImGui::InputText("Text: ", &text))
		{
			ChangeText(text);
		}
	}
	if (!created)
		this->_gameObject->DestroyComponent(this);
}

void TextRendererComponent::OnTransformCallback(float4x4 worldMatrix)
{
	float3 position = worldMatrix.TranslatePart();
	GetTextObject().position = { position.x, position.y };
}

void TextRendererComponent::CreateText()
{
	textID = Application::Instance()->renderer3D->renderManager.AddTextObject();
}

void TextRendererComponent::Serialization(json& j)
{
	json _j;

	_j["Type"] = _type;

	TextObject& textObject = GetTextObject();

	_j["PositionX"] = textObject.position.x;
	_j["PositionY"] = textObject.position.y;

	_j["Scale"] = textObject.scale;

	_j["Color"] = { textObject.color.x, textObject.color.y, textObject.color.z, textObject.color.w };

	_j["Text"] = textObject.text;

	_j["Enabled"] = textObject.draw;

	j["Components"].push_back(_j);
}

void TextRendererComponent::DeSerialization(json& j)
{
	TextObject& textObject = GetTextObject();

	textObject.text = j["Text"];
	textObject.scale = j["Scale"];

	textObject.position.x = j["PositionX"];
	textObject.position.y = j["PositionY"];

	std::vector<float> jColor = j["Color"];
	textObject.color.x = jColor[0];
	textObject.color.y = jColor[1];
	textObject.color.z = jColor[2];
	textObject.color.w = jColor[3];

	text = textObject.text;
	scale = textObject.scale;
	color = textObject.color;
	_gameObject->transform->SetPosition({ textObject.position.x, textObject.position.y, 0 });
}

void TextRendererComponent::ChangeColor(float4 newColor)
{
	GetTextObject().color = newColor;
}

void TextRendererComponent::ChangeScale(float scale)
{
	GetTextObject().scale = scale;
}

void TextRendererComponent::ChangeText(const std::string& text)
{
	GetTextObject().text = text;
}

void TextRendererComponent::OnEnable()
{
	GetTextObject().draw = true;
}

void TextRendererComponent::OnDisable()
{
	GetTextObject().draw = false;
}

TextObject& TextRendererComponent::GetTextObject()
{
	return Application::Instance()->renderer3D->renderManager.textObjects[textID];
}
