#pragma once

//#include "json.hpp"
//
//using json = nlohmann::json;


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
	Uniform(){};
	~Uniform(){};

	void Create();

	virtual void SetVariable(){};
#ifdef STANDALONE
	virtual void GUI(){};
#endif

public:
	UniformData data;
};


class UniBool : Uniform
{
public:
	UniBool(){};
	UniBool(UniformData data) { this->data = data; }
	~UniBool()
	{
		bool* b = static_cast<bool*>(data.value);
		RELEASE(b);
	};

public:
	void SetVariable() override;
#ifdef STANDALONE
	void GUI() override;
#endif
};

class UniInt : Uniform
{
public:
	UniInt() {};
	UniInt(UniformData data) { this->data = data; }
	~UniInt() 
	{
		int* i = static_cast<int*>(data.value);
		RELEASE(i);
	};

public:
	void SetVariable() override;
#ifdef STANDALONE
	void GUI() override;
#endif
};

class UniUInt : Uniform
{
public:
	UniUInt() {};
	UniUInt(UniformData data) { this->data = data; }
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
};

class UniFloat : Uniform
{
public:
	UniFloat() {};
	UniFloat(UniformData data) { this->data = data; }
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
};

class UniFloat2 : Uniform
{
public:
	UniFloat2() {};
	UniFloat2(UniformData data) { this->data = data; }
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
};

class UniFloat3 : Uniform
{
public:
	UniFloat3() {};
	UniFloat3(UniformData data) { this->data = data; }
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
};

class UniFloat4 : Uniform
{
public:
	UniFloat4() {};
	UniFloat4(UniformData data) { this->data = data; }
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
};

class UniDouble : Uniform
{
public:
	UniDouble() {};
	UniDouble(UniformData data) { this->data = data; }
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
};

class UniSampler2D : Uniform
{
public:
	UniSampler2D() {};
	UniSampler2D(UniformData data) { this->data = data; }
	~UniSampler2D() 
	{
		/*Texture* s2d = static_cast<Texture*>(value);

		if (!s2d->resUuid.empty())
		{
			ResourceTexture* res = (ResourceTexture*)ResourceProperties::Instance()->resources.at(s2d->resUuid);
			if (res != nullptr)
			{
				res->DecreaseRC();
			}
		}
		else RELEASE(s2d);*/
	};

public:
	void SetVariable() override;
#ifdef STANDALONE
	void GUI() override;
#endif
};

class UniFloat4x4 : Uniform
{
public:
	UniFloat4x4() {};
	UniFloat4x4(UniformData data) { this->data = data; }
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
};