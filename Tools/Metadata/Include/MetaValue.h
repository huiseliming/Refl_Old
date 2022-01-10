#pragma once
#include "Metadata.h"

class CType;

struct METADATA_API CQualifiedType
{
public:
    CQualifiedType(CType* type = nullptr, uint64_t flag = 0)
        : Type(type)
        , Flag(flag)
    {}
    
    bool operator==(const CQualifiedType& qualifiedType)
    {
        return Type == qualifiedType.Type && Flag == qualifiedType.Flag;
    }

    CType* Type;
    uint64_t Flag;
};



class CGlobalValue : public CMetadata
{
public:
    CGlobalValue(const std::string& name, const CQualifiedType* qualifiedType, void* ptr)
        : CMetadata(name)
        , QualifiedType(qualifiedType)
        , Ptr(ptr)
    {
    }
    const CQualifiedType* QualifiedType;
    void* Ptr;
};

class CField : public CMetadata
{
public:
    CField(const std::string& name, const CQualifiedType* qualifiedType, CType* ownerType, size_t offset = 0)
        : CMetadata(name)
        , QualifiedType(qualifiedType)
        , OwnerType(ownerType)
        , Offset(offset)
    {}

    const CQualifiedType* QualifiedType;
    CType* OwnerType;
    size_t Offset;
};

class CParameter : public CMetadata
{
public:
    CParameter(const std::string& name, const CQualifiedType* qualifiedType)
        : CMetadata(name)
        , QualifiedType(qualifiedType)
    {}

    // Return (*Function)(Arg0, Arg1, Arg2, Arg3 ... )
    //    |                |     |     |     |   ...  
    //    |                |     |     |     |   ...  Index Mapping
    //    V                V     V     V     V   ...  
    // [ -1 ] (*Function)([0],  [1] , [2] , [3]  ... )
    const CQualifiedType* QualifiedType;
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
