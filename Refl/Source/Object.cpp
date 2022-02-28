#include "Object.h"
#include "Archive.h"




void RObject::Serialize(CArchive& Ar)
{
	SerializeProperties(Ar);
}

void RObject::SerializeProperties(CArchive& Ar)
{
	Ar.SerializeClass(this, GetClass());
	//CClass* Class = GetClass();
	//while (Class)
	//{
	//	for (size_t i = 0; i < Class->GetProperties().size(); i++)
	//	{
	//		CProperty* Property = Class->GetProperties()[i];
	//		uint32_t PropertyNameSize = Property->GetName().size();
	//		//Ar.Serialize(&PropertyNameSize, sizeof(uint32_t));
	//		//Ar.Serialize((void*)Property->GetName().c_str(), PropertyNameSize);
	//		// TODO : compatible endianness[big-endian, little-endian]
	//		if (!Property->HasAnyFlag(EPF_PointerFlag | EPF_ReferenceFlag))
	//		{
	//			if (EPF_BoolFlag | Property->GetTypeFlag()) Ar.Serialize(Property->GetRowPtr(this), sizeof(bool));
	//			else if (EPF_SInt8Flag | Property->GetTypeFlag()) Ar.Serialize(Property->GetRowPtr(this), sizeof(int8_t));
	//			else if (EPF_SInt16Flag | Property->GetTypeFlag()) Ar.Serialize(Property->GetRowPtr(this), sizeof(int16_t));
	//			else if (EPF_SInt32Flag | Property->GetTypeFlag()) Ar.Serialize(Property->GetRowPtr(this), sizeof(int32_t));
	//			else if (EPF_SInt64Flag | Property->GetTypeFlag()) Ar.Serialize(Property->GetRowPtr(this), sizeof(int64_t));
	//			else if (EPF_UInt8Flag | Property->GetTypeFlag()) Ar.Serialize(Property->GetRowPtr(this), sizeof(uint8_t));
	//			else if (EPF_UInt16Flag | Property->GetTypeFlag()) Ar.Serialize(Property->GetRowPtr(this), sizeof(uint16_t));
	//			else if (EPF_UInt32Flag | Property->GetTypeFlag()) Ar.Serialize(Property->GetRowPtr(this), sizeof(uint32_t));
	//			else if (EPF_UInt64Flag | Property->GetTypeFlag()) Ar.Serialize(Property->GetRowPtr(this), sizeof(uint64_t));
	//			else if (EPF_FloatFlag | Property->GetTypeFlag()) Ar.Serialize(Property->GetRowPtr(this), sizeof(float));
	//			else if (EPF_DoubleFlag | Property->GetTypeFlag()) Ar.Serialize(Property->GetRowPtr(this), sizeof(double));
	//			else if (EPF_StringFlag | Property->GetTypeFlag())
	//			{
	//				auto StringPtr = (std::string*)Property->GetRowPtr(this);
	//				uint32_t StrSize = StringPtr->size();
	//				Ar.Serialize(&StrSize, sizeof(uint32_t));
	//				Ar.Serialize((void*)StringPtr->c_str(), StrSize);
	//			}
	//			else if (EPF_ClassFlag | Property->GetTypeFlag())
	//			{
	//				Ar.SerializeProperties(Property->GetClass(), Property->GetRowPtr(this));
	//			}
	//			else if (EPF_EnumFlag)
	//			{
	//				Ar.Serialize(Property->GetRowPtr(this), Property->GetEnum()->GetSize());
	//			}
	//		}
	//		else
	//		{
	//			// ObjectPtr will serialize by uuid 
	//			if (EPF_ObjectFlag | Property->GetTypeFlag())
	//			{
	//				if (Ar.IsWriter())
	//				{
	//					Ar << Property->GetObjectPtr(this)->GetUUID();
	//				}
	//				else
	//				{
	//					std::string ObjectUUID;
	//					Ar << ObjectUUID;
	//					if (CObject* Object = FindObject(ObjectUUID))
	//					{
	//						Property->SetObjectPtr(Object, Object);
	//					}
	//				}
	//			}
	//		}
	//	}
	//	// loop to base
	//	if (Class->GetBases().size() > 0 || Class->GetBases()[0] != nullptr)
	//	{
	//		Class = Class->GetBases()[0];
	//	}
	//}
}
//
//void RObject::Register()
//{
//	assert(!UUID_.empty());
//	std::lock_guard<std::mutex> Lock(IDToObjectMutex);
//	IDToObject[UUID_] = this;
//}
//
//void RObject::Unregister()
//{
//	std::lock_guard<std::mutex> Lock(IDToObjectMutex);
//	assert(IDToObject.contains(UUID_));
//	IDToObject.erase(UUID_);
//}
//
RObject* RObject::FindObject(std::string UUID)
{
	//auto OI = IDToObject.find(UUID);
	//if (OI != IDToObject.end())
	//{
	//	return OI->second;
	//}
	return nullptr;
}
//
//
//CObjectManager& CObjectManager::Instance()
//{
//	static CObjectManager ObjectManager;
//	return ObjectManager;
//}

RObject* NewObject(CClass* Cls)
{
	SetCurrentThreadObjectManager();
	FObjectItem& ObjectItem = GThreadObjectManager->ApplyObjectItem();
	ObjectItem.ObjectPtr_ = (RObject*)Cls->New();
	ObjectItem.ObjectPtr_->SetClass(Cls);
	ObjectItem.UUID_ = GUUIDSystemGenerator();
	return ObjectItem.ObjectPtr_;
}