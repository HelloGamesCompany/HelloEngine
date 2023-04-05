#pragma once
#include "ComponentUI.h"
#include "FontManager.h"

class TextRendererComponent : public ComponentUI
{
public:
	TextRendererComponent(GameObject* go);
	~TextRendererComponent();

#ifdef STANDALONE
	void OnEditor() override;

	void MarkAsDead() override;
#endif

	void OnTransformCallback(float4x4 worldMatrix) override;

	void CreateText();

	void Serialization(json& j) override;
	void DeSerialization(json& j) override;

	void ChangeColor(float4 newColor);
	void ChangeScale(float scale);
	void ChangeText(const std::string& text);

	void OnEnable();
	void OnDisable();

	TextObject& GetTextObject();

private:
	float4 color = { 0,0,0,1 };
	float scale = 1.0f;
	std::string text;
	uint textID = 0;
};

