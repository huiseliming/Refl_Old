#pragma once
#include "Metadata.h"

class CQualifiedType;

class CCppType;

class CProperty : public CMetadata
{
    CProperty(const std::string& name, const CQualifiedType* qualifiedType = nullptr, size_t offset = 0)
        : CMetadata(name)
    {}

    virtual void* GetVariableAddress() = 0;



};

//
//template<typename CppType>
//class TCppField : public CMetaValue
//{
//public:
//    TField(const std::string& name, void* ptr)
//        : CMetaValue(name, ptr, sizeof(CppType))
//    {}
//
//};
