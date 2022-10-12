#pragma once
#include "ImWindow.h"
class ImWindowHierarchy : public ImWindow
{
public:

	ImWindowHierarchy();

	~ImWindowHierarchy();

	void Update() override;
};

