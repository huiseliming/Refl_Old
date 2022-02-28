#include "GCObject.h"
#include "Object.h"
#include "Property.h"

std::unordered_map<std::thread::id, CThreadObjectManager> GThreadIdToThreadObjectManager;
CThreadObjectManager* GThreadObjectManager;
uuids::uuid_system_generator GUUIDSystemGenerator;

void CThreadObjectManager::CollectGarbage()
{
	// clear gc mark
	for (auto It = UsedObjectItemList_.begin(); It != UsedObjectItemList_.end(); It++)
	{
		ObjectArray_[*It].ObjectFlag_ &= ~EROF_GCMark;
	}
	// mark
	CollectGarbageMark();

	// sweep
	CollectGarbageSweep();
}

void CThreadObjectManager::CollectGarbageMark()
{
	for (auto It = GCRootSet_.begin(); It != GCRootSet_.end(); It++)
	{
		CollectGarbageMark(ObjectArray_[*It].ObjectPtr_);
	}
}

void CThreadObjectManager::CollectGarbageMark(RObject* O)
{
	if (!O) return;
	if (ObjectArray_[O->ObjectItemIndex_].ObjectFlag_ & EROF_GCMark) return;
	ObjectArray_[O->ObjectItemIndex_].ObjectFlag_ |= EROF_GCMark;
	for (int32_t i = 0; i < O->GetClass()->GetProperties().size(); i++)
	{
		CProperty* Property = O->GetClass()->GetProperties()[i];
		if (Property->HasAnyFlag(EPF_ObjectFlag))
		{
			CollectGarbageMark(Property->GetReflObject(O));
		}
		else if (Property->HasAnyFlag(EPF_ClassFlag) && !Property->IsPointer())
		{
			CollectGarbageMark(Property->GetClass(), Property->GetRowPtr(O));
		}
		else if (Property->HasAnyFlag(EPF_VectorFlag))
		{
			CVectorTemplateType* VectorTemplateType = Property->GetTemplateInstantiationType();
			CProperty* DataProperty = Property->GetDataProperty();
			if (DataProperty->HasAnyFlag(EPF_ObjectFlag))
			{
				for (size_t i = 0; i < VectorTemplateType->GetSize(Property->GetRowPtr(O)); i++)
				{
					void* DataPtr = VectorTemplateType->GetDataPtr(Property->GetRowPtr(O), i);
					CollectGarbageMark(DataProperty->GetReflObject(DataPtr));
				}
			}
			else if (DataProperty->HasAnyFlag(EPF_ClassFlag) && !DataProperty->IsPointer())
			{
				for (size_t i = 0; i < VectorTemplateType->GetSize(Property->GetRowPtr(O)); i++)
				{
					void* DataPtr = VectorTemplateType->GetDataPtr(Property->GetRowPtr(O), i);
					CollectGarbageMark(DataProperty->GetClass(), DataProperty->GetRowPtr(DataPtr));
				}
			}
		}
	}
}

void CThreadObjectManager::CollectGarbageMark(CClass* Cls, void* RowPtr)
{
	for (int32_t i = 0; i < Cls->GetProperties().size(); i++)
	{
		CProperty* Property = Cls->GetProperties()[i];
		if (Property->HasAnyFlag(EPF_ObjectFlag))
		{
			CollectGarbageMark(Property->GetReflObject(RowPtr));
		}
		else if (Property->HasAnyFlag(EPF_ClassFlag) && !Property->IsPointer())
		{
			CollectGarbageMark(Property->GetClass(), Property->GetRowPtr(RowPtr));
		}
		else if (Property->HasAnyFlag(EPF_VectorFlag))
		{
			CVectorTemplateType* VectorTemplateType = Property->GetTemplateInstantiationType();
			CProperty* DataProperty = Property->GetDataProperty();
			if (DataProperty->HasAnyFlag(EPF_ObjectFlag))
			{
				for (size_t i = 0; i < VectorTemplateType->GetSize(Property->GetRowPtr(RowPtr)); i++)
				{
					void* DataPtr = VectorTemplateType->GetDataPtr(Property->GetRowPtr(RowPtr), i);
					CollectGarbageMark(DataProperty->GetReflObject(DataPtr));
				}
			}
			else if (DataProperty->HasAnyFlag(EPF_ClassFlag) && !DataProperty->IsPointer())
			{
				for (size_t i = 0; i < VectorTemplateType->GetSize(Property->GetRowPtr(RowPtr)); i++)
				{
					void* DataPtr = VectorTemplateType->GetDataPtr(Property->GetRowPtr(RowPtr), i);
					CollectGarbageMark(DataProperty->GetClass(), DataProperty->GetRowPtr(DataPtr));
				}
			}
		}
	}
}

void CThreadObjectManager::CollectGarbageSweep()
{
	for (auto It = UsedObjectItemList_.begin(); It != UsedObjectItemList_.end(); It++)
	{
		if (!(ObjectArray_[*It].ObjectFlag_ & EROF_GCMark))
		{
			//ObjectArray_[*It].ObjectFlag_ |= EROF_NeedDelete;
			CollectGarbageSweep(ObjectArray_[*It].ObjectPtr_);
		}
	}
}

void CThreadObjectManager::CollectGarbageSweep(RObject* O)
{
	FObjectItemIndexType ObjectItemIndex = O->ObjectItemIndex_;
	delete O;
	ReleaseObjectItem(ObjectItemIndex);
}

void SetCurrentThreadObjectManager()
{
	GThreadObjectManager = &GThreadIdToThreadObjectManager[std::this_thread::get_id()];
}

void CollectGarbage()
{
	SetCurrentThreadObjectManager();
	GThreadObjectManager->CollectGarbage();
}

std::string ToString(uuids::uuid UUID)
{
	return uuids::to_string(UUID);
}
