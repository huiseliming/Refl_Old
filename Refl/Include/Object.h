#pragma once 
#include "Class.h"

class REFL_API CObject
{
public:
	CObject(CClass* Class = nullptr)
		: Class_(Class)
	{}
	virtual ~CObject() {}

	CClass* GetClass() { return Class_; }
	void SetClass(CClass* Class) { Class_ = Class; }
private:
	CClass* Class_;
};

CObject* REFL_API NewObject(CClass* Class);


