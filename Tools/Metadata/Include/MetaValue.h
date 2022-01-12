#pragma once
#include "Metadata.h"

class CQualifiedType;

class CType;

class CField : public CMetadata
{
public:
    CField(const std::string& name, const CQualifiedType* qualifiedType = nullptr, size_t offset = 0)
        : CMetadata(name)
        , QualifiedType(qualifiedType)
        , Offset(offset)
    {}

    const CQualifiedType* QualifiedType;
    size_t Offset;
};

class CParameter : public CMetadata
{
public:
    CParameter(const std::string& name, const CQualifiedType* qualifiedType, uint32_t index)
        : CMetadata(name)
        , QualifiedType(qualifiedType)
        , Index(index)
    {}

    // Return (*Function)(Arg0, Arg1, Arg2, Arg3 ... ArgN)
    //    |                |     |     |     |   ...  |   
    //    |                |     |     |     |   ...  |   Index Mapping
    //    V                V     V     V     V   ...  V   
    // [ N+1 ] (*Function)([0],  [1] , [2] , [3]  ... [N] )
    const CQualifiedType* QualifiedType;
    uint32_t Index;
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
