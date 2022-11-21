#pragma once
#include "Component.h"
#include "MathGeoLib.h"

struct TransformValues
{
	float3 position = { 0,0,0 };
	float3 scale = {1.0f,1.0f,1.0f};
	float3 rotation = { 0,0,0 };

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

	void SetTransform(float4x4& localTransformMatrix);

	void SetLocalFromGlobal(float4x4& globalMatrix);

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

	float4x4 GetGlobalMatrix(bool forceUpdate = false);

	float3 GetForward();
	float3 GetRight();
	float3 GetUp();

	void OnEditor() override;

	void ForceUpdate();

	bool CheckDirtyFlag() { return _dirtyFlag; }

private:
	void UpdateDirtyFlagForChildren();

	void CalculateLocalMatrix();

	void UpdateDirtyFlag();

private:
	float4x4 _globalMatrix = float4x4::identity;
	float4x4 _localMatrix = float4x4::identity;

	TransformValues localTransform;

	TransformValues tempTransform;

	bool _dirtyFlag = true;
	bool _calculateLocal = true;

	friend class GameObject;
};