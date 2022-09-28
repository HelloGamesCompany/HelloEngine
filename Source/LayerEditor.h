#ifndef __MODULE_ENGINE_H__
#define __MODULE_ENGINE_H__

#include "ImWindow.h"
#include "Layer.h"

enum class ImWindowID
{
	CONFIGURATION,
	ABOUT,
	OPENGL,
	MAX,
};

class LayerEditor :public Layer
{
public:
	LayerEditor();
	~LayerEditor();

	void Start() override;

	void PreUpdate() override;
	void Update() override;
	void PostUpdate() override;

	void CleanUp() override;

private:

	ImWindow* imWindows[(uint)ImWindowID::MAX] = { nullptr };
};

#endif // !__MODULE_ENGINE_H__