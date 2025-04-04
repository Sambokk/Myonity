#pragma once
#include "GameObject.h"
#include "TypeInfoGenerate.h"

class Component : public GameObject
{
public:
	Component();
	~Component();

public:
	virtual void Initialize();
	virtual void BeginPlay();
	virtual void Tick(float deltaTime);
	virtual void EndPlay();

private:

};

