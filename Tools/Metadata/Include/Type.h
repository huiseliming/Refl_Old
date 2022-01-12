#pragma once
#include "Metadata.h"
#include "MetaValue.h"
#include "Function.h"

class METADATA_API CType : public CMetadata
{

public:
    CType(const std::string& name, uint32_t InSize = 0)
        : CMetadata(name)
        , Size(InSize)
    {
        //Manager().RegisterType(this);
    }

    void AddField(CField* InField)
    {
        Fields.push_back(InField);
    }

private:
    uint32_t Size;
    std::vector<CType*       > BaseTypes;
    //std::vector<CConstructor*> Constructors;
    std::vector<CField*      > Fields;
    std::vector<CFunction*   > Functions;
};

enum EQualifiedTypeFlagBit : uint32_t
{
    EQTFB_Unqualfied = 0x0,
    EQTFB_CppType    = 0x0,
    EQTFB_Const      = 0x1,
    EQTFB_Volatile   = 0x2,
    EQTFB_CV         = 0x3,
    EQTFB_Pointer    = 0x4,
    EQTFB_Reference  = 0x8,
};

struct METADATA_API CQualifiedType
{
public:
    virtual ~CQualifiedType() = default;
    bool IsCppType()      { return QualifiedFlag == EQTFB_CppType; }
    bool IsCVType()       { return (QualifiedFlag & EQTFB_CV) == EQTFB_CV; }
    bool IsConstType()    { return QualifiedFlag & EQTFB_Const; }
    bool IsVolatileType() { return QualifiedFlag & EQTFB_Volatile; }
    bool IsPointer()      { return QualifiedFlag & EQTFB_Pointer; }
    bool IsReference()    { return QualifiedFlag & EQTFB_Reference; }

    CQualifiedType* GetPointee() { return Next.QualifiedType; }
    CQualifiedType* GetReferee() { return Next.QualifiedType; }
    CType*          GetCppType() { return Next.Type; }

    union
    {
        CType* Type;
        CQualifiedType* QualifiedType;
    } Next;
    uint32_t QualifiedFlag{ EQTFB_Unqualfied };
};
