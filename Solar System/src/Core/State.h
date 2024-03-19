#pragma once

class State
{
public:
	State() = default;
	virtual ~State() = default;

	virtual void OnCreate() = 0;
	virtual void OnUpdate(float deltaTime) = 0;
	virtual void OnDraw() = 0;
	virtual void OnDestroy() = 0;
};
