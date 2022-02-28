#pragma once
#include <vector>
#include <string>
#include "Refl.h"

class RObject;

struct REFL_API CArchive
{
public:

	virtual ~CArchive() {}

	virtual void Serialize(void* Ptr, uint64_t Length) = 0;
	void SerializeClass(void* BasePtr, CClass* Class);
	void SerializeProperty(void* BasePtr, CProperty* Property);
	void SerializeProperties(void* BasePtr, std::vector<CProperty*>& Properties);

	// TODO : compatible endianness[big-endian, little-endian]
	template<typename T>
	CArchive& operator <<(T& V)
	{
		if constexpr (std::is_enum_v<T>)
		{
			CEnum* Enum = TEnum<std::remove_cv_t<T>>::StaticEnum();
			Serialize((void*)&V, Enum->GetSize());
		}
		else
			SerializeClass((void*)&V, T::StaticClass());
		return *this;
	}

	CArchive& operator <<(bool& V);
	CArchive& operator <<(uint8_t& V);
	CArchive& operator <<(uint16_t& V);
	CArchive& operator <<(uint32_t& V);
	CArchive& operator <<(uint64_t& V);
	CArchive& operator <<(int8_t& V);
	CArchive& operator <<(int16_t& V);
	CArchive& operator <<(int32_t& V);
	CArchive& operator <<(int64_t& V);
	CArchive& operator <<(float& V);
	CArchive& operator <<(double& V);
	virtual CArchive& operator <<(CClass*& Value) { return *this; };
	virtual CArchive& operator <<(std::string& Value) { return *this; };
	//CArchive& operator <<(std::string&& Value) { return operator <<(Value); };
	template<typename T>
	CArchive& operator <<(std::vector<T>& Value)
	{
		uint32_t Size = Value.size();
		CArchive::operator <<(Size);
		Value.resize(Size);
		for (size_t i = 0; i < Size; i++)
		{
			CArchive::operator <<(static_cast<std::remove_cv_t<T>&>(Value[i]));
		}
		return *this;
	}

	CArchive& operator <<(RObject* Value)
	{
		Value->Serialize(*this);
		return *this;
	}
	virtual bool IsWriter() const { return false; }

protected:

};

struct REFL_API CArchiveWriter : public CArchive
{
public:
	virtual bool IsWriter() const override { return true; }
	const std::vector<uint8_t>& GetData();
	void SwapData(std::vector<uint8_t>& Data);
	virtual void Serialize(void* Ptr, uint64_t Length) override;
	virtual CArchive& operator <<(CClass*& Value) override;
	virtual CArchive& operator<<(std::string& Value) override;
	std::vector<uint8_t> Data_;
};

struct REFL_API CArchiveReader : public CArchive
{
public:
	CArchiveReader(uint8_t* DataPtr)
		: CArchive()
		, DataPtr_(DataPtr)
	{
	}
	void SetDataPtr(uint8_t* DataPtr) { DataPtr_ = DataPtr; }
	void ResetOffset() { Offset_ = 0; }
	virtual void Serialize(void* Ptr, uint64_t Length) override;
	virtual CArchive& operator <<(CClass*& Value) override;
	virtual CArchive& operator <<(std::string& Value) override;
	uint8_t* DataPtr_{ nullptr };
	uint64_t Offset_{0};
};



//template<typename T>
//struct  HasStaticClass
//{
//	template<typename U, CClass* (U::*)()> struct SFINAE {};
//	template<typename U> static uint8_t Test(SFINAE<U, &U::StaticClass>*) {}
//	template<typename U> static int16_t Test(...) {}
//	static constexpr bool Value = sizeof(Test<T>(nullptr)) == sizeof(uint8_t);
//};

