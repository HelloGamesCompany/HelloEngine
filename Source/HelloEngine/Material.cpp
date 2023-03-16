#include "Headers.h"
#include "Material.h"
#include "glew.h"
#include "ModuleResourceManager.h"
#include "Uniform.h"



Material::Material()
{
}

Material::Material(uint UID)
{
	SetShader(UID);
}

Material::~Material()
{
}

void Material::SetShader(uint UID)
{
	//Clean current shader
	if (this->shader != nullptr)
	{
		this->shader->Dereference();
		this->shader = nullptr;
		CleanUniforms();
	}

	this->shader = (ResourceShader*)ModuleResourceManager::S_LoadResource(UID);
	this->shader->shader.UniformParser(uniforms);
	shaderVersion = shader->version;
}

void Material::CleanUniforms()
{
	for (int i = 0; i < uniforms.size(); ++i)
	{
		RELEASE(uniforms[i]);
	}
	uniforms.clear();
}

void Material::CheckVersion()
{
	if (shader == nullptr) return;
	if (shader->version == shaderVersion) return;
	
	//The version change when the shader is recompiled. The Material must ask for the new uniforms
	CleanUniforms();
	this->shader->shader.UniformParser(uniforms);
	shaderVersion = shader->version;
}

void Material::Save()
{
	if (shader == nullptr) return;

	json j;

	j["Shader resource UID"] = shader->UID;

	json uni;

	for (int i = 0; i < uniforms.size(); ++i)
	{
		uniforms[i]->GetJSON(uni);
	}

	j["Uniforms"] = uni;
}

void Material::LoadJSON(std::string filePath)
{
	if (shader == nullptr) return;

	char* data = nullptr;
	ModuleFiles::S_Load(filePath, &data);
	if (data == nullptr)
		return;

	json j = json::parse(data);

	SetShader(j["Shader resource UID"]);

	json uni = j["Uniforms"];

	CleanUniforms();
	for (int i = 0; i < uni.size(); ++i)
	{
		uint type = uni["Type"];

		switch (type)
		{
			case GL_BOOL: uniforms.push_back((Uniform*) new UniBool(uni)); break;
			case GL_INT: uniforms.push_back((Uniform*) new UniInt(uni)); break;
			case GL_UNSIGNED_INT: uniforms.push_back((Uniform*) new UniUInt(uni)); break;
			case GL_FLOAT: uniforms.push_back((Uniform*) new UniFloat(uni)); break;
			case GL_FLOAT_VEC2: uniforms.push_back((Uniform*) new UniFloat2(uni)); break;
			case GL_FLOAT_VEC3: uniforms.push_back((Uniform*) new UniFloat3(uni)); break;
			case GL_FLOAT_VEC4: uniforms.push_back((Uniform*) new UniFloat4(uni)); break;
			case GL_DOUBLE: uniforms.push_back((Uniform*) new UniDouble(uni)); break;
			case GL_SAMPLER_2D: uniforms.push_back((Uniform*) new UniSampler2D(uni)); break;
			case GL_FLOAT_MAT4: uniforms.push_back((Uniform*) new UniFloat4x4(uni)); break;
		}
	}
}