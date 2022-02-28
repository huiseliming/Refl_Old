#pragma once
#include <set>
#include <list>
#include <vector>
#include <unordered_map>
#define UUID_SYSTEM_GENERATOR
#include "uuid.h"

class RObject;

using FUUID = uuids::uuid;
using FObjectItemIndexType = int32_t;

enum EObjectLifecycle
{
	EOL_Invalid,
	EOL_PreInitialize,
	EOL_Initializing,
	EOL_PostInitialize,
	EOL_Valid,
	EOL_PreUninitialize,
	EOL_Uninitializing,
	EOL_PostUninitialize,
	EOL_NumberMax,
};

enum EReflObjectFlag
{
	EROF_,
};


struct FObjectItem
{
	FUUID UUID_;
	RObject* ObjectPtr_{ nullptr };
	uint64_t ObjectLifecycle : 3;
	uint64_t ObjectFlag_ : 8;
};

class CThreadObjectManager
{
public:
	CThreadObjectManager() = default;

	enum {
		MinimumAllocationBlockNumber = 1 * 1024 * 1024,
	};
	using UObjectItemBlockType = std::array<FObjectItem, MinimumAllocationBlockNumber>;
public:

	FObjectItem& ApplyObjectItem()
	{
		if (AvailableObjectItems_.empty()) Expand();
		FObjectItemIndexType ObjectItemIndex = AvailableObjectItems_.front();
		AvailableObjectItems_.pop_front();
		//OccupiedObjectItems_.insert(ObjectItemIndex);
		FObjectItem& Ref = ObjectItemRef(ObjectItemIndex);
		Ref.ObjectLifecycle = EOL_PreInitialize;
		return Ref;
	}

	void ReleaseObjectItem(FObjectItemIndexType ObjectItemIndex)
	{
		FObjectItem& Ref = ObjectItemRef(ObjectItemIndex);
		AvailableObjectItems_.push_front(ObjectItemIndex);
		Ref.ObjectLifecycle = EOL_Invalid;
	}

protected:
	void Expand(FObjectItemIndexType BlockNumber = 1)
	{
		int32_t NewBlockIndex = ObjectItemBlocks_.size();
		ObjectItemBlocks_.push_back({});
		UObjectItemBlockType& ObjectItemBlock = ObjectItemBlocks_.back();
		std::memset(&ObjectItemBlock, 0, sizeof(UObjectItemBlockType));
		// clear memory
		for (size_t i = 0; i < MinimumAllocationBlockNumber; i++)
		{
			FObjectItemIndexType ArrayIndex = NewBlockIndex * MinimumAllocationBlockNumber + i;
			AvailableObjectItems_.push_back(ArrayIndex);
		}
	}

	FObjectItem* ObjectItemPtr(FObjectItemIndexType Index)
	{
		return &ObjectItemBlocks_[Index / MinimumAllocationBlockNumber][Index % MinimumAllocationBlockNumber];
	}

	FObjectItem& ObjectItemRef(FObjectItemIndexType Index)
	{
		return ObjectItemBlocks_[Index / MinimumAllocationBlockNumber][Index % MinimumAllocationBlockNumber];
	}

protected:
	std::vector<UObjectItemBlockType> ObjectItemBlocks_;
	std::list<FObjectItemIndexType> AvailableObjectItems_;
};

extern thread_local CThreadObjectManager GThreadObjectManager;











