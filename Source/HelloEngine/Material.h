#pragma once

class ResourceShader;
class Uniform;

#include "json.hpp"

using json = nlohmann::json;

class Material
{
public:
	Material();
	Material(uint UID);
	~Material();

	void Update(const float* view, const float* projection, const float* model);

	void Clear();

	ResourceShader* GetShader() { return shader; }
	bool SetShader(uint UID);

	void Save(json& j);
	void LoadJSON(std::string filePath);

	void CheckVersion();
private:
	void CleanUniforms();

public:
	std::vector<Uniform*> uniforms;

private:
	ResourceShader* shader = nullptr;

	int shaderVersion;
};

