#pragma once 
#include <set>
#include <mutex>
#include <vector>
#include <array>
#include <list>
#include <unordered_set>
#include "Class.h"
#include "GCObject.h"


class CArchive;


class REFL_API RObject
{
private:
	friend RObject* NewObject(CClass*);
	RObject(CClass* Class = nullptr);
	void SetClass(CClass* Class) { Class_ = Class; }

public:
	virtual ~RObject();
	virtual void Serialize(CArchive& Ar);
	virtual void SerializeProperties(CArchive& Ar);

	void Register();
	void Unregister();

	CClass* GetClass() { return Class_; }
public:

	static RObject* FindObject(std::string UUID);

protected:
	static std::mutex UUIDToObjectMutex;
	static std::unordered_map<FUUID, RObject*> UUIDToObject;

	std::set<RObject*> RootSet;

protected:
	CClass* Class_;
	FObjectItemIndexType ObjectItem_;
};

template<typename T>
RObject* NewObject(const std::string& UUID = {})
{
	return NewObject(T::StaticClass(), UUID);
}

REFL_API RObject* NewObject(CClass* Class, const std::string& UUID = {});

RObject* NewObject(CClass* Cls)
{
	FObjectItem& ObjectItem = GThreadObjectManager.ApplyObjectItem();
	ObjectItem.ObjectPtr_ = (RObject*)Cls->New();
	ObjectItem.ObjectPtr_->SetClass(Cls);
	ObjectItem.UUID_ = uuids::uuid_system_generator{}();
	ObjectItem.ObjectLifecycle = EOL_Initializing;
	ObjectItem.ObjectLifecycle = EOL_PostInitialize;
	ObjectItem.ObjectLifecycle = EOL_Valid;
	return ObjectItem.ObjectPtr_;
}
//
RObject* LoadObject(CArchive& Ar);
