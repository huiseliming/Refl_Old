#include "Archive.h"

void CArchiveWriter::Serialize(void* Ptr, uint64_t Length)
{
	const uint64_t OldSize = Data_.size();
	const uint64_t NewSize = OldSize + /*sizeof(uint64_t) +*/ Length;
	Data_.resize(NewSize);
	//std::memcpy(Data_.data() + OldSize, &Length, sizeof(uint64_t));
	std::memcpy(Data_.data() + OldSize /* + sizeof(uint64_t) */ , Ptr, Length);
}

void CArchiveWriter::SerializeProperties(CClass* Class, void* Ptr)
{
	for (size_t i = 0; i < Class->GetProperties().size(); i++)
	{
		CProperty* Property = Class->GetProperties()[i];
		*this << Property->GetName();
		// TODO : compatible endianness[big-endian, little-endian]
		if      (EPF_BoolFlag   | Property->GetTypeFlag()) Serialize(Property->GetRowPtr(Ptr), sizeof(bool));
		else if (EPF_SInt8Flag  | Property->GetTypeFlag()) Serialize(Property->GetRowPtr(Ptr), sizeof(int8_t));
		else if (EPF_SInt16Flag | Property->GetTypeFlag()) Serialize(Property->GetRowPtr(Ptr), sizeof(int16_t));
		else if (EPF_SInt32Flag | Property->GetTypeFlag()) Serialize(Property->GetRowPtr(Ptr), sizeof(int32_t));
		else if (EPF_SInt64Flag | Property->GetTypeFlag()) Serialize(Property->GetRowPtr(Ptr), sizeof(int64_t));
		else if (EPF_UInt8Flag  | Property->GetTypeFlag()) Serialize(Property->GetRowPtr(Ptr), sizeof(uint8_t));
		else if (EPF_UInt16Flag | Property->GetTypeFlag()) Serialize(Property->GetRowPtr(Ptr), sizeof(uint16_t));
		else if (EPF_UInt32Flag | Property->GetTypeFlag()) Serialize(Property->GetRowPtr(Ptr), sizeof(uint32_t));
		else if (EPF_UInt64Flag | Property->GetTypeFlag()) Serialize(Property->GetRowPtr(Ptr), sizeof(uint64_t));
		else if (EPF_FloatFlag  | Property->GetTypeFlag()) Serialize(Property->GetRowPtr(Ptr), sizeof(float));
		else if (EPF_DoubleFlag | Property->GetTypeFlag()) Serialize(Property->GetRowPtr(Ptr), sizeof(double));
		else if (EPF_StringFlag | Property->GetTypeFlag())
		{
			auto StringPtr = (std::string*)Property->GetRowPtr(Ptr);
			Serialize(StringPtr->data(), StringPtr->size() + 1);
		}
	}
}

//EPF_ZeroFlag = 0ULL,
//EPF_VoidFlag = 1ULL << 0,
//EPF_BoolFlag = 1ULL << 1,
//EPF_SInt8Flag = 1ULL << 2,
//EPF_SInt16Flag = 1ULL << 3,
//EPF_SInt32Flag = 1ULL << 4,
//EPF_SInt64Flag = 1ULL << 5,
//EPF_UInt8Flag = 1ULL << 6,
//EPF_UInt16Flag = 1ULL << 7,
//EPF_UInt32Flag = 1ULL << 8,
//EPF_UInt64Flag = 1ULL << 9,
//EPF_FloatFlag = 1ULL << 10,
//EPF_DoubleFlag = 1ULL << 11,
//EPF_StringFlag = 1ULL << 12,
//EPF_ClassFlag = 1ULL << 13,
//EPF_EnumFlag = 1ULL << 14,
//CArchive& CArchive::operator<<(CObject& Input)
//{
//	
//	CClass* Class = Input.GetClass();
//	for (size_t i = 0; i < Class->GetProperties().size(); i++)
//	{
//		CProperty* Property = Class->GetProperties()[i];
//		Property
//		if (Property->GetFlag() | 
//				(
//					EPF_BoolFlag   |
//					EPF_SInt8Flag  |
//					EPF_SInt16Flag |
//					EPF_SInt32Flag |
//					EPF_SInt64Flag |
//					EPF_UInt8Flag  |
//					EPF_UInt16Flag |
//					EPF_UInt32Flag |
//					EPF_UInt64Flag |
//					EPF_FloatFlag  |
//					EPF_DoubleFlag
//				)
//			)
//		{
//			;
//		}
//	}
//	return *this;
//}
