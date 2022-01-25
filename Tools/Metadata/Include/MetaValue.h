#pragma once
#include "Metadata.h"

class CType;

class CProperty : public CMetadata
{
    CProperty(const std::string& name)
        : CMetadata(name)
        , Type(nullptr)
        , Flag(0)
        , AddressOffset(0)
    {
        offsetof(CProperty, AddressOffset);
    }

    virtual void* GetVariableAddress() = 0;
protected:
    CType* Type;
    uint64_t Flag;
    uint32_t AddressOffset;
};

template<typename T>
class TCppProperty : public CProperty
{
    TCppProperty(const std::string& name)
        :CProperty(name)
    {
        Type = StaticType<T>();
    }
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
