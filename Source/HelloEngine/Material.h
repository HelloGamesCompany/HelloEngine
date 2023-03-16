#pragma once

class ResourceShader;
class Uniform;

class Material
{
public:
	Material();
	Material(uint UID);
	~Material();

	ResourceShader* GetShader() { return shader; }
	void SetShader(uint UID);

	void Save();
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

