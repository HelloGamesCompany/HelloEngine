#include "Headers.h"
#include "Lighting.h"

GlobalLight Lighting::global;
LightMap Lighting::_lightMap;

Lighting::Lighting()
{
	_active = true;
	ref = -1;
}

Lighting::~Lighting()
{

}

//uint Lighting::AddLight(Light lightData)
//{
//	uint _lightID = 0;
//
//	switch (lightData.GetType())
//	{
//	case Component::Type::DIRECTIONAL_LIGHT:
//		_lightMap.directionalLight = (DirectionalLight) lightData;
//		break;
//	case Component::Type::POINT_LIGHT:
//		_lightID = _lightMap.pointLight.size();
//		_lightMap.pointLight[_lightID] = (PointLightComponent::PointLight) lightData;
//		break;
//	case Component::Type::SPOT_LIGHT:
//		_lightID = _lightMap.spotLight.size();
//		_lightMap.spotLight[_lightID] = (SpotLightComponent::SpotLight) lightData;
//		break;
//	}
//
//	return _lightID;
//}

void Lighting::SetDirectionalLight(DirectionalLight lightData)
{
	_lightMap.directionalLight = lightData;
}

uint Lighting::AddPointLight(PointLight lightData)
{
	uint _lightID = 0;

	_lightID = _lightMap.pointLight.size();
	_lightMap.pointLight[_lightID] = lightData;

	return _lightID;
}

uint Lighting::AddSpotLight(SpotLight lightData)
{
	uint _lightID = 0;

	_lightID = _lightMap.spotLight.size();
	_lightMap.spotLight[_lightID] = lightData;

	return _lightID;
}

void Lighting::Update()
{
	if (!_active) return;

	//float pos[] = { global.position.x, global.position.y, global.position.z, 1.0f };
	//glLightfv(ref, GL_POSITION, pos);
}