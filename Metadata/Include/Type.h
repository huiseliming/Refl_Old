#pragma once
#include <functional>
#include "Metadata.h"

class CClass;
class CEnum;

class METADATA_API CType : public CReflRecord
{
public:
    CType(const std::string& InName, uint32_t InSize = 0)
        : CReflRecord(InName)
        , Size_(InSize)
    {
    }
    void SetSize(uint32_t Size) { Size_ = Size; }
    uint32_t GetSize() { return Size_; }

    static std::unordered_map<std::string, CType*>& StaticTable();
    static std::unordered_map<std::string, CType*>& NameToType;
    static std::list<std::function<void()>>& PostStaticInitializerList();
    static void PostStaticInitializer();
private:
    uint32_t Size_;
};

template<typename T>
struct TAutoInitializer
{
    TAutoInitializer()
    {
        CClass* Class = T::StaticClass();
        assert(!CType::StaticTable().contains(Class->GetName()));
        CType::StaticTable().insert(std::make_pair(Class->GetName(), Class));
    }
};

template<typename T>
struct TEnum
{
    static CEnum* StaticEnum();
    static CEnum* ENUM_STATIC_INITIALIZER();
};

//
//enum EQualifiedTypeFlagBit : uint32_t
//{
//    EQTFB_Unqualfied = 0x0,
//    EQTFB_CppType    = 0x0,
//    EQTFB_Const      = 0x1,
//    EQTFB_Volatile   = 0x2,
//    EQTFB_CV         = 0x3,
//    EQTFB_Pointer    = 0x4,
//    EQTFB_Reference  = 0x8,
//};
//
//class CTypeManager
//{
//    CTypeManager();
//public:
//    static CTypeManager& Instance();
//
//
//
//
//    std::unordered_map<std::string, std::shared_ptr<CQualifiedType>> NameToQualifiedType;
//    std::unordered_map<std::string, std::shared_ptr<CCppType>> NameToCppType;
//
//};

//
//struct METADATA_API CQualifiedType
//{
//public:
//    virtual ~CQualifiedType() = default;
//    bool IsCppType()      { return QualifiedFlag == EQTFB_CppType; }
//    bool IsCVType()       { return (QualifiedFlag & EQTFB_CV) == EQTFB_CV; }
//    bool IsConstType()    { return QualifiedFlag & EQTFB_Const; }
//    bool IsVolatileType() { return QualifiedFlag & EQTFB_Volatile; }
//    bool IsPointer()      { return QualifiedFlag & EQTFB_Pointer; }
//    bool IsReference()    { return QualifiedFlag & EQTFB_Reference; }
//
//    CQualifiedType* GetPointee() { return Next.QualifiedType; }
//    CQualifiedType* GetReferee() { return Next.QualifiedType; }
//    CCppType*          GetCppType() { return Next.Type; }
//
//    union
//    {
//        CCppType* Type;
//        CQualifiedType* QualifiedType;
//    } Next;
//    uint32_t QualifiedFlag{ EQTFB_Unqualfied };
//};
