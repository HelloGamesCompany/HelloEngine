#pragma once

// Class for ordering objects inside the Engine, so they can be updated in an specific order.
// Called "Layer" because the input reding of the objects will depend on the position of the Layer they are into.
// Superior Layers will get input state before inferior Layers.
// This is mainly to avoid problems when Scripts and GUI read the Input state at the same time, but do not share the same priority.
class Layer
{
public:

	Layer();
	virtual ~Layer();

	virtual void Start();

	virtual void PreUpdate();
	virtual void Update();
	virtual void PostUpdate();

	virtual void CleanUp();

	void SetEnabled(bool enabled)
	{
		_isEnabled = enabled;
	}

	bool IsEnabled() const
	{
		return _isEnabled;
	}

private:
	bool _isEnabled = true;

};

