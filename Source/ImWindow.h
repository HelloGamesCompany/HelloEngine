#pragma once

#include <string>

class ImWindow
{
public:

	ImWindow() {};
	virtual ~ImWindow() {};

	virtual void Update() {};

	bool isEnabled = true;
	std::string windowName = "DefaultWindow";

};

