#pragma once

#include "ImWindow.h"

class ImWindowAbout : public ImWindow
{
public:
	ImWindowAbout();

	void Update() override;

private:
	std::string sdlVersion;
	std::string glewVersion;
	std::string imGUIVersion;
	std::string jsonVersion;
	std::string mathGeoLibVersion;
	std::string pugiXmlVersion;
};
