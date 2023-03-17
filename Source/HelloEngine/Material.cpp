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

void Material::Update(const float* view, const float* projection, const float* model)
{
	shader->shader.Bind();

	shader->shader.SetMatFloat4v("view", view);
	shader->shader.SetMatFloat4v("projection", projection);
	shader->shader.SetMatFloat4v("model", model);

	for (uint i = 0; i < uniforms.size(); ++i)
	{
		uniforms[i]->Update(shader->shader);
	}
}

bool Material::SetShader(uint UID)
{
	//Clean current shader
	if (this->shader != nullptr)
	{
		this->shader->Dereference();
		this->shader = nullptr;
		CleanUniforms();
	}

	this->shader = (ResourceShader*)ModuleResourceManager::S_LoadResource(UID);

	if (this->shader != nullptr)
	{
		this->shader->shader.UniformParser(uniforms);
		shaderVersion = shader->version;
		return true;
	}
	return false;
}

void Material::Clear()
{
	CleanUniforms();
	if (shader == nullptr) return;
	shader->Dereference();
	shader = nullptr;
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

void Material::Save(json& j)
{
	if (shader == nullptr) return;

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
	char* data = nullptr;
	ModuleFiles::S_Load(filePath, &data);
	if (data == nullptr)
		return;

	json j = json::parse(data);

	if (!j.contains("Shader resource UID")) return;
	if (!SetShader(j["Shader resource UID"])) return;

	json uni = j["Uniforms"];

	CleanUniforms();
	for (int i = 0; i < uni.size(); ++i)
	{
		uint type = uni[i]["Type"];

		switch (type)
		{
			case GL_BOOL: uniforms.push_back((Uniform*) new UniBool(uni[i])); break;
			case GL_INT: uniforms.push_back((Uniform*) new UniInt(uni[i])); break;
			case GL_UNSIGNED_INT: uniforms.push_back((Uniform*) new UniUInt(uni[i])); break;
			case GL_FLOAT: uniforms.push_back((Uniform*) new UniFloat(uni[i])); break;
			case GL_FLOAT_VEC2: uniforms.push_back((Uniform*) new UniFloat2(uni[i])); break;
			case GL_FLOAT_VEC3: uniforms.push_back((Uniform*) new UniFloat3(uni[i])); break;
			case GL_FLOAT_VEC4: uniforms.push_back((Uniform*) new UniFloat4(uni[i])); break;
			case GL_DOUBLE: uniforms.push_back((Uniform*) new UniDouble(uni[i])); break;
			case GL_SAMPLER_2D: uniforms.push_back((Uniform*) new UniSampler2D(uni[i])); break;
			case GL_FLOAT_MAT4: uniforms.push_back((Uniform*) new UniFloat4x4(uni[i])); break;
		}
	}
}