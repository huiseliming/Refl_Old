#include "Object.h"
#include "Archive.h"
#define UUID_SYSTEM_GENERATOR
#include "uuid.h"
std::mutex CObject::UUIDToObjectMutex;
std::unordered_map<std::string, CObject*> CObject::UUIDToObject;

CObject::~CObject() 
{
	Unregister();
}

void CObject::SetUUID(const std::string& UUID)
{
	{
		std::lock_guard<std::mutex> Lock(UUIDToObjectMutex);
		assert(!UUIDToObject.contains(UUID_));
	}
	UUID_ = UUID;
}

const std::string& CObject::GetUUID()
{ 
	return UUID_;
}

void CObject::Serialize(CArchive& Ar)
{
	Ar << UUID_;
	SerializeProperties(Ar);
}

void CObject::SerializeProperties(CArchive& Ar)
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

void CObject::Register()
{
	assert(!UUID_.empty());
	std::lock_guard<std::mutex> Lock(UUIDToObjectMutex);
	UUIDToObject[UUID_] = this;
}

void CObject::Unregister()
{
	std::lock_guard<std::mutex> Lock(UUIDToObjectMutex);
	assert(UUIDToObject.contains(UUID_));
	UUIDToObject.erase(UUID_);
}

CObject* CObject::FindObject(std::string UUID)
{
	auto OI = UUIDToObject.find(UUID);
	if (OI != UUIDToObject.end())
	{
		return OI->second;
	}
	return nullptr;
}

CObject* NewObject(CClass* Class, const std::string& UUID)
{
	CObject* O = (CObject*)Class->New();
	O->SetClass(Class);
	if (!UUID.empty()) O->SetUUID(UUID);
	else O->SetUUID(uuids::to_string(uuids::uuid_system_generator{}()));
	return O;
}

