#include "Headers.h"
#include "Material.h"
#include "glew.h"
#include "ModuleResourceManager.h"
#include "Uniform.h"

#include "ModuleCamera3D.h"
#include "CameraObject.h"

#include "Lighting.h"

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

void Material::UpdateBones(std::vector<float4x4>& bones)
{
	for (int i = 0; i < bones.size(); ++i)
	{
		shader->shader.SetMatFloat4v("finalBonesMatrices[" + std::to_string(i) + "]", &bones[i].Transposed().v[0][0]);
	}
}

void Material::UpdateLights()
{
	LightMap& lightMap = Lighting::GetLightMap();

	//Directional Light
	if (lightMap.directionalLight.isEnabled)
	{
		shader->shader.SetFloat3v("Light_Directional.Base.Color", &lightMap.directionalLight.color.At(0));
		shader->shader.SetFloat("Light_Directional.Base.AmbientIntensity", lightMap.directionalLight.ambientIntensity);
		shader->shader.SetFloat("Light_Directional.Base.DiffuseIntensity", lightMap.directionalLight.diffuseIntensity);
		shader->shader.SetFloat3v("Light_Directional.Direction", &lightMap.directionalLight.direction.At(0));
	}
	
	//Point Light
	for (int i = 0; (i < lightMap.pointLight.size()) && (i < shader->shader.data._maxPointLights); ++i)
	{
		if (!lightMap.pointLight[i].isEnabled) continue;
		shader->shader.SetFloat3v("Light_Point[" + std::to_string(i) + "].Base.Color", &lightMap.pointLight[i].color.At(0));
		shader->shader.SetFloat("Light_Point[" + std::to_string(i) + "].Base.AmbientIntensity", lightMap.pointLight[i].ambientIntensity);
		shader->shader.SetFloat("Light_Point[" + std::to_string(i) + "].Base.DiffuseIntensity", lightMap.pointLight[i].diffuseIntensity);

		shader->shader.SetFloat("Light_Point[" + std::to_string(i) + "].Constant", lightMap.pointLight[i].constant);
		shader->shader.SetFloat("Light_Point[" + std::to_string(i) + "].Linear", lightMap.pointLight[i].linear);
		shader->shader.SetFloat("Light_Point[" + std::to_string(i) + "].Exponential", lightMap.pointLight[i].exp);
		shader->shader.SetFloat3v("Light_Point[" + std::to_string(i) + "].Position", &lightMap.pointLight[i].position.At(0));
	}

	//Spot Light
	for (int i = 0; (i < lightMap.spotLight.size()) && i < (shader->shader.data._maxSpotLights); ++i)
	{
		if (!lightMap.spotLight[i].isEnabled) continue;
		shader->shader.SetFloat3v("Light_Spot[" + std::to_string(i) + "].Base.Color", &lightMap.spotLight[i].color.At(0));
		shader->shader.SetFloat("Light_Spot[" + std::to_string(i) + "].Base.AmbientIntensity", lightMap.spotLight[i].ambientIntensity);
		shader->shader.SetFloat("Light_Spot[" + std::to_string(i) + "].Base.DiffuseIntensity", lightMap.spotLight[i].diffuseIntensity);
	
		shader->shader.SetFloat("Light_Spot[" + std::to_string(i) + "].Constant", lightMap.spotLight[i].constant);
		shader->shader.SetFloat("Light_Spot[" + std::to_string(i) + "].Linear", lightMap.spotLight[i].linear);
		shader->shader.SetFloat("Light_Spot[" + std::to_string(i) + "].Exponential", lightMap.spotLight[i].exp);
		shader->shader.SetFloat("Light_Spot[" + std::to_string(i) + "].Cutoff", lightMap.spotLight[i].cutoff);
		shader->shader.SetFloat3v("Light_Spot[" + std::to_string(i) + "].Position", &lightMap.spotLight[i].position.At(0));
		shader->shader.SetFloat3v("Light_Spot[" + std::to_string(i) + "].Direction", &lightMap.spotLight[i].direction.At(0));
	}

	shader->shader.SetInt("Actual_Spot", lightMap.spotLight.size());
	shader->shader.SetInt("Actual_Point", lightMap.pointLight.size());
}

void Material::Update(const float* view, const float* projection, const float* model)
{
	shader->shader.Bind();

	shader->shader.SetMatFloat4v("view", view);
	shader->shader.SetMatFloat4v("projection", projection);
	shader->shader.SetMatFloat4v("model", model);

	for (uint i = 0; i < uniforms.size(); ++i)
	{
		//If true, the uniform was Key and has already been Handled (Given Data)
		if (HandleKeyUniforms(uniforms[i])) continue;

		uniforms[i]->Update(shader->shader);
	}

	//Update Engine lights if the shader uses them
	if (shader->shader.data.hasEngineLight) UpdateLights();
}

void Material::UpdateInstanced(const float* view, const float* projection)
{
	shader->shader.Bind();

	shader->shader.SetMatFloat4v("view", view);
	shader->shader.SetMatFloat4v("projection", projection);

	for (uint i = 0; i < uniforms.size(); ++i)
	{
		//If true, the uniform was Key and has already been Handled (Given Data)
		if (HandleKeyUniforms(uniforms[i])) continue;


		uniforms[i]->Update(shader->shader);
	}
}

void Material::UnbindAllTextures()
{
	for (uint i = 0; i < uniforms.size(); ++i)
	{
		if (uniforms[i]->data.type != GL_SAMPLER_2D) continue;

		UniSampler2D* sampler = (UniSampler2D*)uniforms[i];
		sampler->Unbind();
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

bool Material::HandleKeyUniforms(Uniform* uni)
{
	bool toReturn = false;

	//By Type and name
	switch (uni->data.type)
	{
		case GL_FLOAT_VEC3:
			if (uni->data.name == "LightColor")
			{
				shader->shader.SetFloat3v("LightColor", &Lighting::global.lightColor.At(0));
				toReturn = true;
			}
			else if (uni->data.name == "ViewPoint")
			{
				float3 viewPoint = Application::Instance()->camera->currentDrawingCamera->GetPosition();
				shader->shader.SetFloat3v("ViewPoint", &viewPoint.At(0));

				toReturn = true;
			}
			break;
		case GL_FLOAT_VEC4:
			if (uni->data.name == "LightPosition")
			{
				shader->shader.SetFloat4v("LightPosition", &Lighting::global.lightDirection.At(0));
				
				toReturn = true;
			}
			break;
		case GL_FLOAT:
			if (uni->data.name == "LightStrength")
			{
				shader->shader.SetFloat3v("LightStrength", &Lighting::global.lightStrength);
				
				toReturn = true;
			}
			else if (uni->data.name == "Actual_Spot" || uni->data.name == "Actual_Point")
			{
				toReturn = true;
			}

			break;
		case GL_FLOAT_MAT4:
			if (uni->data.name == "finalBonesMatrices")
			{
				toReturn = true;
			}
			break;
	}

	return toReturn;
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

	RELEASE_ARRAY(data);

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