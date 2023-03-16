#pragma once

#include "json.hpp"

using json = nlohmann::json;


struct UniformData
{
	uint index;
	std::string name;
	std::string strType;

	int read, size;
	uint type;

	void* value = nullptr;
};

class Uniform
{
public:
	Uniform() {};
	~Uniform(){};

	virtual void SetVariable(){};
#ifdef STANDALONE
	virtual void GUI(){};
#endif

	void GetJSON(json& j);
	void SetJSON(json& j)
	{
		data.name = j["Name"];
		data.type = j["Type"];
		data.strType = j["String Type"];
		data.index = j["Index"];
		data.read = j["Read"];
		data.size = j["Size"];
	}

private:
	virtual void GetJSONUnique(json& _j){};
public:
	UniformData data;
};


class UniBool : Uniform
{
public:
	UniBool(){};
	UniBool(json& j)
	{
		SetJSON(j);
		bool* b = new bool(j["Value"]);
		data.value = b;
	}
	UniBool(UniformData data) 
	{ 
		this->data = data;
		SetVariable();
	}
	~UniBool()
	{
		bool* b = static_cast<bool*>(data.value);
		RELEASE(b);
	};

	void SetVariable() override;
#ifdef STANDALONE
	void GUI() override;
#endif
private:
	void GetJSONUnique(json& _j) override;
};

class UniInt : Uniform
{
public:
	UniInt() {};
	UniInt(json& j)
	{
		SetJSON(j);
		int* i = new int(j["Value"]);
		data.value = i;
	}
	UniInt(UniformData data) 
	{ 
		this->data = data; 
		SetVariable();
	}
	~UniInt() 
	{
		int* i = static_cast<int*>(data.value);
		RELEASE(i);
	};

	void SetVariable() override;
#ifdef STANDALONE
	void GUI() override;
#endif
private:
	void GetJSONUnique(json& _j) override;
};

class UniUInt : Uniform
{
public:
	UniUInt() {};
	UniUInt(json& j) 
	{
		SetJSON(j);
		uint* ui = new uint(j["Value"]);
		data.value = ui;
	}
	UniUInt(UniformData data) 
	{ 
		this->data = data; 
		SetVariable();
	}
	~UniUInt()
	{
		uint* ui = static_cast<uint*>(data.value);
		RELEASE(ui);
	};

public:
	void SetVariable() override;
#ifdef STANDALONE
	void GUI() override;
#endif
private:
	void GetJSONUnique(json& _j) override;
};

class UniFloat : Uniform
{
public:
	UniFloat() {};
	UniFloat(json& j)
	{
		SetJSON(j);
		float* f = new float(j["Value"]);
		data.value = f;
	}
	UniFloat(UniformData data) 
	{ 
		this->data = data; 
		SetVariable();
	}
	~UniFloat() 
	{
		float* f = static_cast<float*>(data.value);
		RELEASE(f);
	};

public:
	void SetVariable() override;
#ifdef STANDALONE
	void GUI() override;
#endif
private:
	void GetJSONUnique(json& _j) override;
};

class UniFloat2 : Uniform
{
public:
	UniFloat2() {};
	UniFloat2(json& j)
	{
		SetJSON(j);
		float2* f2 = new float2();

		json aux = j["Value"];

		for (int i = 0; i < aux.size(); ++i)
		{
			f2->At(i) = aux[i];
		}
		data.value = f2;
	}
	UniFloat2(UniformData data) 
	{ 
		this->data = data;
		SetVariable();
	}
	~UniFloat2() 
	{
		float2* f2 = static_cast<float2*>(data.value);
		RELEASE(f2);
	};

public:
	void SetVariable() override;
#ifdef STANDALONE
	void GUI() override;
#endif
private:
	void GetJSONUnique(json& _j) override;
};

class UniFloat3 : Uniform
{
public:
	UniFloat3() {};
	UniFloat3(json& j)
	{
		SetJSON(j);
		float3* f3 = new float3();

		json aux = j["Value"];

		for (int i = 0; i < aux.size(); ++i)
		{
			f3->At(i) = aux[i];
		}
		data.value = f3;
	}
	UniFloat3(UniformData data) 
	{ 
		this->data = data; 
		SetVariable();
	}
	~UniFloat3() 
	{
		float3* f3 = static_cast<float3*>(data.value);
		RELEASE(f3);
	};

public:
	void SetVariable() override;
#ifdef STANDALONE
	void GUI() override;
#endif
private:
	void GetJSONUnique(json& _j) override;
};

class UniFloat4 : Uniform
{
public:
	UniFloat4() {};
	UniFloat4(json& j)
	{
		SetJSON(j);
		float4* f4 = new float4();

		json aux = j["Value"];

		for (int i = 0; i < aux.size(); ++i)
		{
			f4->At(i) = aux[i];
		}
		data.value = f4;
	}
	UniFloat4(UniformData data) 
	{ 
		this->data = data; 
		SetVariable();
	}
	~UniFloat4() 
	{
		float4* f4 = static_cast<float4*>(data.value);
		RELEASE(f4);
	};

public:
	void SetVariable() override;
#ifdef STANDALONE
	void GUI() override;
#endif
private:
	void GetJSONUnique(json& _j) override;
};

class UniDouble : Uniform
{
public:
	UniDouble() {};
	UniDouble(json& j)
	{
		SetJSON(j);
		double* d = new double(j["Value"]);
		data.value = d;
	}
	UniDouble(UniformData data) 
	{ 
		this->data = data;
		SetVariable();
	}
	~UniDouble() 
	{
		double* d = static_cast<double*>(data.value);
		RELEASE(d);
	};

public:
	void SetVariable() override;
#ifdef STANDALONE
	void GUI() override;
#endif
private:
	void GetJSONUnique(json& _j) override;
};

class UniSampler2D : Uniform
{
public:
	UniSampler2D() {};
	UniSampler2D(json& j); //On CPP
	UniSampler2D(UniformData data) 
	{ 
		this->data = data; 
		SetVariable();
	}
	
	~UniSampler2D(); //On CPP

public:
	void SetVariable() override;
#ifdef STANDALONE
	void GUI() override;
#endif
private:
	void GetJSONUnique(json& _j) override;
};

class UniFloat4x4 : Uniform
{
public:
	UniFloat4x4() {};
	UniFloat4x4(json& j)
	{
		SetJSON(j);
		float4x4* f4x4 = new float4x4();

		json aux = j["Value"];

		int i = 0;
		for (int x = 0; x < 4; ++x)
		{
			for (int y = 0; y < 4; ++y)
			{
				f4x4->At(x, y) = aux[i];
				++i;
			}
		}

		data.value = f4x4;
	}
	UniFloat4x4(UniformData data) 
	{
		this->data = data; 
		SetVariable();
	}
	~UniFloat4x4() 
	{
		float4x4* f44 = static_cast<float4x4*>(data.value);
		RELEASE(f44);
	};

public:
	void SetVariable() override;
#ifdef STANDALONE
	void GUI() override;
#endif
private:
	void GetJSONUnique(json& _j) override;
};