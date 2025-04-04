#pragma once
#include "GameObject.h"
class Component;

class Actor : public GameObject
{
public:
	Actor();
	~Actor();

public:
	virtual void Initialize();
	virtual void BeginPlay();
	virtual void Tick(float deltaTime);
	virtual void EndPlay();

public:
	template <typename T>
	T* GetComponent()
	{
		for ( Component* component : m_components )
		{
			if ( auto concrete = Cast<T>(component) )
			{
				return concrete;
			}
		}

		return nullptr;
	}

private:
	vector<Component*> m_components;
};

