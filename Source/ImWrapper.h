#pragma once

class ImWrapper
{
public:

	ImWrapper();

	virtual ~ImWrapper();

	virtual void Update() = 0;

	std::string name = "";
};