#pragma once
#include "ImWrapper.h"
class ImWrapperInputBox :public ImWrapper
{
public:
	ImWrapperInputBox(std::string name);

	~ImWrapperInputBox();

	void Update() override;
};