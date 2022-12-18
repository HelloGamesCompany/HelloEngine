#pragma once
#include "ImWindow.h"
class ImWindowQuickSave :  public ImWindow
{
public:
	ImWindowQuickSave();

	~ImWindowQuickSave();

	void Update() override;
};

