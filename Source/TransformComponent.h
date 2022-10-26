#pragma once
#include "Component.h"
#include "MathGeoLib.h"

struct TransformValues
{
	float3 position;
	float3 scale = {1.0f,1.0f,1.0f};
	float3 rotation;

	bool operator !=(const TransformValues& v)
	{
		return (position != v.position) || (scale != v.scale) || (rotation != v.rotation);
	}

	bool operator ==(const TransformValues& v)
	{
		return (position == v.position) && (scale == v.scale) && (rotation == v.rotation);
	}
};

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

	void SetTransform(float3 pos, float3 scale, float3 rot);

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

	void OnPositionUpdate(float3 pos) override;
	void OnRotationUpdate(float3 rot) override;
	void OnScaleUpdate(float3 scale) override;
	void OnTransformUpdate(float3 pos, float3 scale, float3 rot) override;

	TransformValues GetGlobalTransform();

	float3 GetForward();
	float3 GetRight();
	float3 GetUp();

	void OnEditor() override;

private:
	void UpdatePosition();
	void UpdateRotation();
	void UpdateScale();
	void UpdateTransform();

	void CalculateGlobalMatrix();

private:
	float4x4 globalMatrix;

	TransformValues parentGlobalTransform;
	TransformValues localTransform;

	TransformValues tempTransform;

	friend class GameObject;
};