#pragma once
#include "P_Module.h"

class P_MainModule : public P_Module
{
public:

	P_MainModule();
	~P_MainModule();

#ifdef STANDALONE
	void OnEditor() override;
#endif
};

