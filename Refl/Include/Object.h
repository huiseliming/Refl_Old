#pragma once 
#include <mutex>
#include "Class.h"

class CArchive;

class REFL_API CObject
{
public:
	CObject(CClass* Class = nullptr)
		: Class_(Class)
	{}

	virtual ~CObject();
	virtual void Serialize(CArchive& Ar);
	virtual void SerializeProperties(CArchive& Ar);

	CClass* GetClass() { return Class_; }
	void SetClass(CClass* Class) { Class_ = Class; }

	void SetUUID(const std::string& UUID);
	const std::string& GetUUID();

	void Register();
	void Unregister();
	
public:
	static CObject* FindObject(std::string UUID);

protected:
	static std::mutex UUIDToObjectMutex;
	static std::unordered_map<std::string, CObject*> UUIDToObject;

protected:
	CClass* Class_;
	std::string UUID_;
};

REFL_API CObject* NewObject(CClass* Class, const std::string& UUID = {});


