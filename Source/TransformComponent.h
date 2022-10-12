#pragma once
#include "Component.h"
#include "MathGeoLib.h"

class TransformComponent : public Component
{
public:
	TransformComponent(GameObject* gameObject);
	~TransformComponent();

	/// <summary>
	/// Sets a new position, ignoring the current.
	/// </summary>
	void SetPosition(float3 pos);
	/// <summary>
	/// Sets a new scale, ignoring the current.
	/// </summary>
	void SetScale(float3 s);
	/// <summary>
	/// Sets a new Rotation, ignoring the current.
	/// </summary>
	void SetRotation(float3 rot);

	/// <summary>
	/// Current position is transalted by the given vector
	/// </summary>
	void Translate(float3 translation);

	/// <summary>
	/// Current scale is changed by the given values
	/// </summary>
	void Scale(float3 s);

	/// <summary>
	/// Current rotation is changed by the given values
	/// </summary>
	void Rotate(float3 rotate);

private:
	// TODO: This should give the new Position for every component that has suscribed to this transform updates.
	void CallbackPositions();

private:
	float3 position;
	float3 scale;
	float3 rotation;


};

