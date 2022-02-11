#pragma once
#include <vector>
#include <string>
#include "Refl.h"

class CObject;

struct CArchive
{
public:

	virtual ~CArchive() {}

	virtual void Serialize(void* Ptr, uint64_t Length) = 0;

	virtual void SerializeProperties(CClass* Class, void* Ptr) = 0;

	virtual CArchive& operator<<(const std::string& V)
	{
		Serialize((void*)V.c_str(), V.size() + 1);
		return *this;
	}

	//friend CArchive& operator<<(CArchive& A, const std::string& V);
protected:
	std::vector<uint8_t> Data_;
};

struct CArchiveWriter : public CArchive
{
public:
	virtual void Serialize(void* Ptr, uint64_t Length) override;

	virtual void SerializeProperties(CClass* Class, void* Ptr) override;
};

struct CArchiveReader : public CArchive
{
public:
	virtual void Serialize(void* Ptr, uint64_t Length) override;

	virtual void SerializeProperties(CClass* Class, void* Ptr) override;
};


CArchive& operator<<(CArchive& A, const std::string& V)
{
	A.Serialize((void*)V.c_str(), V.size() + 1);
}

// TODO : compatible endianness[big-endian, little-endian]
template<typename T, typename = typename std::enable_if_t<std::is_arithmetic_v<T>>>
CArchive& operator <<(CArchive& A, T& V) 
{
	A.Serialize(&V, sizeof(V));
	return A;
};

//template<typename T>
//CArchive& TSerializeImpl(CArchive& A, T& V)
//{
//	A.Serialize(&V, sizeof(V));
//	return A;
//}
//
//#define SERIALIZE_TEMPLATE_IMPLEMENTATION(Type) CArchive& operator<<(CArchive& A, Type& V) { return TSerializeImpl<Type>(A, V); };
//
//SERIALIZE_TEMPLATE_IMPLEMENTATION(bool);
//SERIALIZE_TEMPLATE_IMPLEMENTATION(uint8_t);
//SERIALIZE_TEMPLATE_IMPLEMENTATION(uint16_t);
//SERIALIZE_TEMPLATE_IMPLEMENTATION(uint32_t);
//SERIALIZE_TEMPLATE_IMPLEMENTATION(uint64_t);
//SERIALIZE_TEMPLATE_IMPLEMENTATION(int8_t);
//SERIALIZE_TEMPLATE_IMPLEMENTATION(int16_t);
//SERIALIZE_TEMPLATE_IMPLEMENTATION(int32_t);
//SERIALIZE_TEMPLATE_IMPLEMENTATION(int64_t);
//SERIALIZE_TEMPLATE_IMPLEMENTATION(float);
//SERIALIZE_TEMPLATE_IMPLEMENTATION(double);
