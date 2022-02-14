#pragma once
#include <functional>
#include "Record.h"

struct CArchive;
class CClass;
class CEnum;

typedef void* (*FPReflNew)        ();
typedef void  (*FPReflDelete)     (void*);
typedef void  (*FPReflConstructor)(void*);
typedef void  (*FPReflDestructor) (void*);
typedef void  (*FPReflAssign)     (void*, void*);



class REFL_API CType : public CRecord
{
public:
    CType(const std::string& InName, uint32_t InSize = 0)
        : CRecord(InName)
        , Size_(InSize)
    {
    }
    void SetSize(uint32_t Size) { Size_ = Size; }
    uint32_t GetSize() { return Size_; }

    static CType* FindType(const std::string& Name) { return NameToType.contains(Name) ? NameToType[Name] : nullptr; }
    static std::unordered_map<std::string, CType*>& StaticTable();
    static std::unordered_map<std::string, CType*>& NameToType;
    static std::list<std::function<void()>>& PostStaticInitializerEventList();
    static void ProcessPostStaticInitializerEvent();

    FPReflNew         New        { nullptr };
    FPReflDelete      Delete     { nullptr };
    FPReflConstructor Constructor{ nullptr };
    FPReflDestructor  Destructor { nullptr };
    FPReflAssign      CopyAssign { nullptr };
    FPReflAssign      MoveAssign { nullptr };
private:
    uint32_t Size_;
};

template<typename T>
struct TEnum
{
    static CEnum* StaticEnum();
    static CEnum* ENUM_STATIC_INITIALIZER();
};

template<typename T>
struct TAutoInitializer
{
    TAutoInitializer()
    {
        CType* Type = nullptr;
        if constexpr (std::is_enum_v<T>) Type = TEnum<T>::StaticEnum();
        else  Type = T::StaticClass();
        if constexpr (std::is_default_constructible_v<T>)
        {
            Type->New = [] () -> void*{ return new T; };
            Type->Constructor = [](void* A) { new (A)T; };
        }
        Type->Delete = [](void* A) { delete (T*)A; };
        Type->Destructor  = [] (void* A) { ((const T *)(A))->~T(); };
        if constexpr (std::is_copy_assignable_v<T>)
            Type->CopyAssign = [] (void* A, void* B) { (*(T*)A) = (*(const T*)B); };
        if constexpr (std::is_move_assignable_v<T>)
            Type->MoveAssign = [](void* A, void* B) { (*(T*)A) = std::move(*(T*)B); };
        assert(!CType::StaticTable().contains(Type->GetName()));
        CType::StaticTable().insert(std::make_pair(Type->GetName(), Type));
    }
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
