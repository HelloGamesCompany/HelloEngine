#include "Headers.h"
#include "Uniform.h"

/*Variable Set*/

void UniBool::SetVariable()
{
	data.strType = "bool";
	bool* b = new bool(false);
	data.value = b;
}

void UniInt::SetVariable()
{
	data.strType = "int";
	int* i = new int(0);
	data.value = i;
}

void UniUInt::SetVariable()
{
	data.strType = "uint";
	uint* ui = new uint(0);
	data.value = ui;
}

void UniFloat::SetVariable()
{
	data.strType = "float";
	float* f = new float(0.0f);
	data.value = f;
}

void UniFloat2::SetVariable()
{
	data.strType = "vec2";
	float2* f2 = new float2();
	data.value = f2;
}

void UniFloat3::SetVariable()
{
	data.strType = "vec3";
	float3* f3 = new float3();
	data.value = f3;
}

void UniFloat4::SetVariable()
{
	data.strType = "vec4";
	float4* f4 = new float4();
	data.value = f4;
}

void UniDouble::SetVariable()
{
	data.strType = "double";
	double* d = new double(1.0);
	data.value = d;
}

void UniSampler2D::SetVariable()
{
	data.strType = "sampler2D";
	//Texture* s2d = new Texture();
	//value = s2d;
}

void UniFloat4x4::SetVariable()
{
	data.strType = "mat4";
	float4x4* f44 = new float4x4(float4x4::identity);
	data.value = f44;
}

/*HANDLE IMGUI INSPECTOR*/
#ifdef STANDALONE
void UniBool::GUI()
{
	ImGui::Checkbox(data.name.c_str(), static_cast<bool*>(data.value));
}

void UniInt::GUI()
{
	ImGui::InputInt(data.name.c_str(), static_cast<int*>(data.value));
}

void UniUInt::GUI()
{
	ImGui::InputInt(data.name.c_str(), static_cast<int*>(data.value), 0, 4294967295);
}

void UniFloat::GUI()
{
	if (data.name == "deltaTime" || data.name == "DeltaTime" || data.name == "dt") return;
	ImGui::InputFloat(data.name.c_str(), static_cast<float*>(data.value));
}

void UniFloat2::GUI()
{
	ImGui::InputFloat2(data.name.c_str(), &static_cast<float2*>(data.value)->At(0));
}

void UniFloat3::GUI()
{
	if (data.name.find("Colour") || data.name.find("Color") || data.name.find("color") || data.name.find("colour"))
	{
		std::string popName = "ColorPicker##" + data.index;
		float4 color = { *static_cast<float3*>(data.value), 1.0f };
		if (ImGui::ColorButton(data.name.c_str(), (ImVec4&)color, 0, { 50, 50 }))
		{
			ImGui::OpenPopup(popName.c_str());
		}
		ImGui::SameLine();
		ImGui::Text(data.name.c_str());

		if (ImGui::BeginPopup(popName.c_str()))
		{
			ImGui::ColorPicker3(data.name.c_str(), (float*) static_cast<float3*>(data.value), 0 | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);

			ImGui::EndPopup();
		}

	}
	else ImGui::InputFloat3(data.name.c_str(), &static_cast<float3*>(data.value)->At(0));
}

void UniFloat4::GUI()
{
	if (!data.name.find("Colour") || !data.name.find("Color") || !data.name.find("color") || !data.name.find("colour"))
	{
		float4 color = *static_cast<float4*>(data.value);
		if (ImGui::ColorButton(data.name.c_str(), (ImVec4&)color, 0, { 50, 50 }))
		{
			ImGui::OpenPopup("ColorPicker");
		}
		ImGui::SameLine();
		ImGui::Text(data.name.c_str());

		if (ImGui::BeginPopup("ColorPicker"))
		{
			ImGui::ColorPicker4(data.name.c_str(), (float*) static_cast<float4*>(data.value), 0 | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);

			ImGui::EndPopup();
		}
	}
	else ImGui::InputFloat4(data.name.c_str(), &static_cast<float4*>(data.value)->At(0));
}

void UniDouble::GUI()
{
	ImGui::InputDouble(data.name.c_str(), static_cast<double*>(data.value));
}

void UniSampler2D::GUI()
{
	//Texture* tex = static_cast<Texture*>(value);

	//ImGui::ImageButton((ImTextureID)tex->id, { 75, 75 });
	//if (ImGui::BeginDragDropTarget())
	//{
	//	if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentBrowserItem"))
	//	{
	//		IM_ASSERT(payload->DataSize == sizeof(LibraryItem));
	//		const LibraryItem item = *static_cast<const LibraryItem*>(payload->Data);

	//		switch (str2int(item.extension.c_str()))
	//		{
	//		case str2int("png"):
	//		case str2int("PNG"):
	//		case str2int("DDS"):
	//		case str2int("dds"):
	//		case str2int("jpg"):
	//		case str2int("JPG"):
	//		case str2int("tga"):
	//		case str2int("TGA"):

	//			ResourceTexture* res = (ResourceTexture*)ResourceProperties::Instance()->resources.at(item.resUuid);

	//			//Clean current
	//			VariableDeleting();

	//			TextureImporter::ImportFromLibrary(res);
	//			value = res->texture;
	//			tex = static_cast<Texture*>(value);
	//		}
	//	}
	//}
	//ImGui::SameLine();
	//ImGui::Text("%s", name.c_str());
	//std::string txt = "Size: " + std::to_string(tex->w) + " x " + std::to_string(tex->h);
	//ImGui::Text(txt.c_str());
	//txt = "Texture ID: " + std::to_string(tex->id);
	//ImGui::Text(txt.c_str());
}

void UniFloat4x4::GUI()
{
	if (data.name == "Projection" || data.name == "View" || data.name == "Model") return;
	std::string aux;
	float4x4* f44 = static_cast<float4x4*>(data.value);
	aux = "1: " + data.name;
	ImGui::InputFloat4(aux.c_str(), &f44->v[0][0]);
	aux = "2: " + data.name;
	ImGui::InputFloat4(aux.c_str(), &f44->v[1][0]);
	aux = "3: " + data.name;
	ImGui::InputFloat4(aux.c_str(), &f44->v[2][0]);
	aux = "4: " + data.name;
	ImGui::InputFloat4(aux.c_str(), &f44->v[3][0]);
}
#endif