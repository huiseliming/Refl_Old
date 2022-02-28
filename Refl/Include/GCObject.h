#pragma once
#include <set>
#include <list>
#include <vector>
#include <thread>
#include <unordered_map>
#include "ReflExport.h"
#define NOMINMAX
#define UUID_SYSTEM_GENERATOR
#include "uuid.h"

class RObject;
class CClass;
class CProperty;

using FUUID = uuids::uuid;
using FObjectItemIndexType = int32_t;

typedef void FScopeCycleStatOutput(double);

class REFL_API CScopeCycleStat
{
public:
	static FScopeCycleStatOutput ScopeCycleStatOutput;

	CScopeCycleStat()
	{
		Start = std::chrono::steady_clock::now();
	}
	~CScopeCycleStat() {
	
	}
	std::chrono::steady_clock::time_point Start;
};

//class CScopeCycleCounter
//{
//};

enum EReflObjectFlags : uint32_t
{
	EROF_NoFlags     = 0x00000000,
	EROF_MarkGCRoot  = 0x00000001,

	EROF_NeedDelete      = 0x80000000,
	EROF_GCMark		 = 0x80000000,
};

struct FObjectItem
{
	FUUID UUID_;
	RObject* ObjectPtr_{ nullptr };
	uint64_t ObjectFlag_ {0};
	//uint32_t ObjectLifecycle;
};

class REFL_API CThreadObjectManager
{
public:
	CThreadObjectManager() = default;

	enum {
		MinimumAllocationBlockNumber = 1 * 1024 * 1024,
	};
	//using UObjectItemBlockType = std::array<FObjectItem, MinimumAllocationBlockNumber>;
public:

	FObjectItem& ApplyObjectItem()
	{
		if (AvailableObjectItemList_.empty()) Expand();
		FObjectItemIndexType ObjectItemIndex = AvailableObjectItemList_.front();
		AvailableObjectItemList_.pop_front();
		UsedObjectItemList_.insert(ObjectItemIndex);
		FObjectItem& Ref = ObjectItemRef(ObjectItemIndex);
		return Ref;
	}

	void ReleaseObjectItem(FObjectItemIndexType ObjectItemIndex)
	{
		FObjectItem& Ref = ObjectItemRef(ObjectItemIndex);
		AvailableObjectItemList_.push_front(ObjectItemIndex);
		UsedObjectItemList_.erase(ObjectItemIndex);
	}

	void CollectGarbage();

protected:
	void CollectGarbageMark();
	void CollectGarbageMark(RObject* O);
	void CollectGarbageMark(CClass* Cls, void* RowPtr);

	void CollectGarbageSweep();
	void CollectGarbageSweep(RObject* O);

protected:
	void Expand(FObjectItemIndexType BlockNumber = 1)
	{
		for (size_t i = 0; i < BlockNumber; i++)
		{
			AvailableObjectItemList_.push_back(ObjectArray_.size());
			ObjectArray_.push_back({});
		}
	}

	FObjectItem* ObjectItemPtr(FObjectItemIndexType Index)
	{
		return &ObjectArray_[Index];
	}

	FObjectItem& ObjectItemRef(FObjectItemIndexType Index)
	{
		return ObjectArray_[Index];
	}

protected:
	std::vector<FObjectItem> ObjectArray_;
	std::list<FObjectItemIndexType> AvailableObjectItemList_;
	std::set<FObjectItemIndexType> UsedObjectItemList_;
	std::set<FObjectItemIndexType> GCRootSet_;

};

extern REFL_API std::unordered_map<std::thread::id, CThreadObjectManager> GThreadIdToThreadObjectManager;
extern REFL_API CThreadObjectManager* GThreadObjectManager;
extern REFL_API uuids::uuid_system_generator GUUIDSystemGenerator;

REFL_API void SetCurrentThreadObjectManager();

REFL_API void CollectGarbage();

REFL_API std::string ToString(uuids::uuid UUID);
