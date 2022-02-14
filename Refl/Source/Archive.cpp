#include "Archive.h"

void CArchive::SerializeClass(void* BasePtr, CClass* Class)
{
	while (Class)
	{
		SerializeProperties(BasePtr, Class->GetProperties());
		// loop to base
		if (Class->GetBases().size() <= 0 || Class->GetBases()[0] == nullptr) break;
		Class = Class->GetBases()[0];
	}
}

void CArchive::SerializeProperties(void* BasePtr, std::vector<CProperty*>& Properties)
{
	for (size_t i = 0; i < Properties.size(); i++)
	{
		CProperty* Property = Properties[i];
		uint32_t PropertyNameSize = Property->GetName().size();
		//Ar.Serialize(&PropertyNameSize, sizeof(uint32_t));
		//Ar.Serialize((void*)Property->GetName().c_str(), PropertyNameSize);
		// TODO : compatible endianness[big-endian, little-endian]
		if (!Property->HasAnyFlag(EPF_PointerFlag | EPF_ReferenceFlag))
		{
			if (EPF_BoolFlag & Property->GetTypeFlag()) Serialize(Property->GetRowPtr(BasePtr), sizeof(bool));
			else if (EPF_SInt8Flag & Property->GetTypeFlag()) Serialize(Property->GetRowPtr(BasePtr), sizeof(int8_t));
			else if (EPF_SInt16Flag & Property->GetTypeFlag()) Serialize(Property->GetRowPtr(BasePtr), sizeof(int16_t));
			else if (EPF_SInt32Flag & Property->GetTypeFlag()) Serialize(Property->GetRowPtr(BasePtr), sizeof(int32_t));
			else if (EPF_SInt64Flag & Property->GetTypeFlag()) Serialize(Property->GetRowPtr(BasePtr), sizeof(int64_t));
			else if (EPF_UInt8Flag & Property->GetTypeFlag()) Serialize(Property->GetRowPtr(BasePtr), sizeof(uint8_t));
			else if (EPF_UInt16Flag & Property->GetTypeFlag()) Serialize(Property->GetRowPtr(BasePtr), sizeof(uint16_t));
			else if (EPF_UInt32Flag & Property->GetTypeFlag()) Serialize(Property->GetRowPtr(BasePtr), sizeof(uint32_t));
			else if (EPF_UInt64Flag & Property->GetTypeFlag()) Serialize(Property->GetRowPtr(BasePtr), sizeof(uint64_t));
			else if (EPF_FloatFlag & Property->GetTypeFlag()) Serialize(Property->GetRowPtr(BasePtr), sizeof(float));
			else if (EPF_DoubleFlag & Property->GetTypeFlag()) Serialize(Property->GetRowPtr(BasePtr), sizeof(double));
			else if (EPF_StringFlag & Property->GetTypeFlag())
			{
				auto StringPtr = (std::string*)Property->GetRowPtr(BasePtr);
				uint32_t StrSize = StringPtr->size();
				Serialize(&StrSize, sizeof(uint32_t));
				StringPtr->resize(StrSize);
				Serialize((void*)StringPtr->c_str(), StrSize);
			}
			else if (EPF_ClassFlag & Property->GetTypeFlag())
			{
				SerializeClass(Property->GetRowPtr(BasePtr), Property->GetClass());
			}
			else if (EPF_EnumFlag & Property->GetTypeFlag())
			{
				Serialize(Property->GetRowPtr(BasePtr), Property->GetEnum()->GetSize());
			}
		}
		else
		{
			// ObjectPtr will serialize by uuid 
			if (EPF_ObjectFlag & Property->GetTypeFlag())
			{
				if (IsWriter())
				{
					operator<<(const_cast<std::string&>(Property->GetObject(BasePtr)->GetUUID()));
				}
				else
				{
					std::string ObjectUUID;
					operator<<(ObjectUUID);
					if (CObject* Object = CObject::FindObject(ObjectUUID))
					{
						Property->SetObject(Object, Object);
					}
				}
			}
		}
	}
}

CArchive& CArchive::operator <<(bool& V)
{
	Serialize((void*)&V, sizeof(V));
	return *this;
}
CArchive& CArchive::operator <<(uint8_t& V)
{
	Serialize((void*)&V, sizeof(V));
	return *this;
}
CArchive& CArchive::operator <<(uint16_t& V)
{
	Serialize((void*)&V, sizeof(V));
	return *this;
}
CArchive& CArchive::operator <<(uint32_t& V)
{
	Serialize((void*)&V, sizeof(V));
	return *this;
}
CArchive& CArchive::operator <<(uint64_t& V)
{
	Serialize((void*)&V, sizeof(V));
	return *this;
}
CArchive& CArchive::operator <<(int8_t& V)
{
	Serialize((void*)&V, sizeof(V));
	return *this;
}
CArchive& CArchive::operator <<(int16_t& V)
{
	Serialize((void*)&V, sizeof(V));
	return *this;
}
CArchive& CArchive::operator <<(int32_t& V)
{
	Serialize((void*)&V, sizeof(V));
	return *this;
}
CArchive& CArchive::operator <<(int64_t& V)
{
	Serialize((void*)&V, sizeof(V));
	return *this;
}
CArchive& CArchive::operator <<(float& V)
{
	Serialize((void*)&V, sizeof(V));
	return *this;
}
CArchive& CArchive::operator <<(double& V)
{
	Serialize((void*)&V, sizeof(V));
	return *this;
}

const std::vector<uint8_t>& CArchiveWriter::GetData()
{
	return Data_;
}

void CArchiveWriter::SwapData(std::vector<uint8_t>& Data)
{
	Data_.swap(Data);
}

void CArchiveWriter::Serialize(void* Ptr, uint64_t Length)
{
	const uint64_t OldSize = Data_.size();
	const uint64_t NewSize = OldSize + Length;
	Data_.resize(NewSize);
	std::memcpy(Data_.data() + OldSize, Ptr, Length);
}

CArchive& CArchiveWriter::operator <<(CClass*& Value)
{
	CArchiveWriter::operator<<(const_cast<std::string&>(Value->GetName()));
	return *this;
};

CArchive& CArchiveWriter::operator <<(std::string& Value)
{
	uint32_t StrSize = Value.size();
	Serialize(&StrSize, sizeof(uint32_t));
	Serialize((void*)Value.c_str(), StrSize);
	return *this;
}

void CArchiveReader::Serialize(void* Ptr, uint64_t Length)
{
	std::memcpy(Ptr, DataPtr_ + Offset_, Length);
	Offset_ += Length;
}

CArchive& CArchiveReader::operator <<(CClass*& Value)
{
	std::string ClassName;
	CArchiveReader::operator<<(ClassName);
	Value = (CClass*)CType::NameToType[ClassName];
	return *this;
};

CArchive& CArchiveReader::operator <<(std::string& Value)
{
	uint32_t StrSize;
	Serialize(&StrSize, sizeof(uint32_t));
	const_cast<std::string&>(Value).resize(StrSize);
	Serialize((void*)Value.c_str(), StrSize);
	return *this;
}

