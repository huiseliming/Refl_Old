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

/**
 * 
 */
class REFL_API RObjectBase
{
protected:
	RObjectBase() = default;
	virtual ~RObjectBase() {};
	void SetClass(CClass* Class) { Class_ = Class; }

public:
	CClass* GetClass() { return Class_; }

protected:
	CClass* Class_;
	FObjectItemIndexType ObjectItemIndex_;
};


/**
 *
 */
class REFL_API RObject : public RObjectBase
{
	friend class CThreadObjectManager;
private:
	friend REFL_API RObject* NewObject(CClass* Class);
	RObject();
public:
	virtual void Serialize(CArchive& Ar);
	virtual void SerializeProperties(CArchive& Ar);

public:

	static RObject* FindObject(std::string UUID);

protected:

};

/**
 *
 */
template<typename T>
RObject* NewObject() { return NewObject(T::StaticClass()); }

/**
 *
 */
REFL_API RObject* NewObject(CClass* Class);

/**
 *
 */
RObject* LoadObject(CArchive& Ar);
