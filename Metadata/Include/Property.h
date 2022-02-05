#pragma once
#include "Metadata.h"
#include "StaticType.h"

class CType;


enum EPropertyFlag : uint64_t
{
	EPF_ZeroFlag                 = 0ULL,
    EPF_VoidFlag                 = 1ULL << 0,
	EPF_BoolFlag                 = 1ULL << 1,
	EPF_SInt8Flag                = 1ULL << 2,
	EPF_SInt16Flag               = 1ULL << 3,
	EPF_SInt32Flag               = 1ULL << 4,
	EPF_SInt64Flag               = 1ULL << 5,
	EPF_UInt8Flag                = 1ULL << 6,
	EPF_UInt16Flag               = 1ULL << 7,
	EPF_UInt32Flag               = 1ULL << 8,
	EPF_UInt64Flag               = 1ULL << 9,
	EPF_FloatFlag                = 1ULL << 10,
	EPF_DoubleFlag               = 1ULL << 11,
	EPF_StringFlag               = 1ULL << 12,
	EPF_ClassFlag                = 1ULL << 13,
	EPF_EnumFlag			     = 1ULL << 14,

	EPF_VectorFlag               = 1ULL << 24,
	EPF_MapFlag                  = 1ULL << 25,
    EPF_UnorderedMapFlag         = 1ULL << 26,

    EPF_UnknowFlag               = 1ULL << 31,

	EPF_PointerFlag              = 1ULL << 32,
	EPF_ReferenceFlag            = 1ULL << 33,
    EPF_ConstFlag                = 1ULL << 34,
    EPF_VolatileFlag             = 1ULL << 35,


    EPF_TypeFlagBits = 0x00000000FFFFFFFF,
    EPF_SIntFlagBits = EPF_SInt8Flag | EPF_SInt16Flag | EPF_SInt32Flag | EPF_SInt64Flag,
    EPF_UIntFlagBits = EPF_UInt8Flag | EPF_UInt16Flag | EPF_UInt32Flag | EPF_UInt64Flag,
    EPF_IntFlagBits = EPF_SIntFlagBits | EPF_UIntFlagBits,
	EPF_FloatFlagBits = EPF_FloatFlag | EPF_DoubleFlag,
};


class CProperty : public CReflRecord
{
public:
    CProperty(const std::string& Name_)
        : CReflRecord(Name_)
        , Flag_(0)
        , AddressOffset_(0)
    {
    }
    uint32_t GetAddressOffset() { return AddressOffset_; }
    void SetAddressOffset(uint32_t AddressOffset) { AddressOffset_ = AddressOffset; }
    void SetFlag(uint64_t Flag) { Flag_ = Flag; }
    void AddFlags(uint64_t Flags) { Flag_ |= Flags; }
    void RemoveFlags(uint64_t Flags) { Flag_ &= ~Flags; }

    uint64_t GetFlag()                { return Flag_; }
    void*    GetRowPtr(void const* ClassPtr) const { return (void*)(((char*)(ClassPtr)) + AddressOffset_); }

    virtual CType* GetType() { return nullptr; }
    virtual CClass* GetClass() { return nullptr; }
    virtual CEnum* GetEnum() { return nullptr; }

    virtual CProperty* GetDataProperty() { return nullptr; }

    virtual bool GetBool(void const* ClassPtr) const { return false; }
    virtual void SetBool(void* ClassPtr, bool Value) const {}

    virtual int64_t GetSInt(void const* ClassPtr) const { return 0; }
    virtual uint64_t GetUInt(void const* ClassPtr) const { return 0; }
    virtual double GetFloat(void const* ClassPtr) const { return 0.f; }
    virtual void SetUInt(void* ClassPtr, uint64_t Value) const {}
    virtual void SetSInt(void* ClassPtr, int64_t Value) const {}
    virtual void SetFloat(void* ClassPtr, double Value) const {}

    virtual std::string GetString(void const* ClassPtr) const { return ""; }
    virtual const char* GetCString(void const* ClassPtr) const { return ""; }
    virtual void SetString(void* ClassPtr, int64_t Value) const {}
    virtual void SetString(void* ClassPtr, uint64_t Value) const {}
    virtual void SetString(void* ClassPtr, double Value) const {}
    virtual void SetString(void* ClassPtr, const char* Value) const {}
    virtual void SetString(void* ClassPtr, const std::string& Value) const {}

    virtual void SetNumericFromString(void* ClassPtr, char const* Value) const {}
    virtual std::string GetNumericToString(void const* ClassPtr) const { return ""; }
    virtual std::string GetBoolToString(void const* ClassPtr) const { return ""; }

protected:
    uint64_t Flag_;
    uint32_t AddressOffset_;
};

template<typename T>
struct TPropertyAccessor
{
    enum
    {
        CppSize = sizeof(T),
        CppAlignment = alignof(T)
    };
    static T const* GetPtr(void const* A) { return (T const*)(A); }
    static T*       GetPtr(void      * A) { return (T      *)(A); }
    static T const& Get   (void const* A) { return *GetPtr(A);  }

    static T GetDefault()               { return T();  }
    static T GetOptional(void const* B) { return B ? Get(B) : GetDefault(); }

    static void Set(void* A, T const& Value) { *GetPtr(A) = Value; }

    static T*   Initialize(void* A) { return new (A) T(); }
    static void Destroy   (void* A) { GetPtr(A)->~T(); }
};

template <typename T>
struct TNumericProperty : public CProperty
{
    using TIPropertyAccessor = TPropertyAccessor<T>;

    TNumericProperty(const std::string& name)
        : CProperty(name)
    {
    }

    virtual void SetUInt(void* ClassPtr, uint64_t Value) const override
    {
        assert(std::is_integral_v<T>);
        TIPropertyAccessor::Set(GetRowPtr(ClassPtr), (T)Value);
    }
    virtual void SetSInt(void* ClassPtr, int64_t Value) const override
    {
        assert(std::is_integral_v<T>);
        TIPropertyAccessor::Set(GetRowPtr(ClassPtr), (T)Value);
    } 
    virtual void SetFloat(void* ClassPtr, double Value) const override
    {
        assert(std::is_floating_point_v<T>);
        TIPropertyAccessor::Set(GetRowPtr(ClassPtr), (T)Value);
    }
    virtual void SetNumericFromString(void* ClassPtr, char const* Value) const override
    {
//#pragma warning(push)
//#pragma warning (disable: 4244)
        *TIPropertyAccessor::GetPtr(GetRowPtr(ClassPtr)) = (T)atoll(Value);
//#pragma warning(pop)
    }    
    virtual std::string GetNumericToString(void const* ClassPtr) const override
    {
        return std::to_string(TIPropertyAccessor::Get(GetRowPtr(ClassPtr)));
    }
    virtual int64_t GetSInt(void const* ClassPtr) const override
    {
        assert(std::is_integral_v<T>);
        return (int64_t)TIPropertyAccessor::Get(GetRowPtr(ClassPtr));
    }
    virtual uint64_t GetUInt(void const* ClassPtr) const override
    {
        assert(std::is_integral_v<T>);
        return (uint64_t)TIPropertyAccessor::Get(GetRowPtr(ClassPtr));
    }
    virtual double GetFloat(void const* ClassPtr) const override
    {
        assert(std::is_floating_point_v<T>);
        return (double)TIPropertyAccessor::Get(GetRowPtr(ClassPtr));
    }

};

class CVoidProperty : public CProperty
{
    using TIPropertyAccessor = TPropertyAccessor<void>;
public:
    CVoidProperty(const std::string& name)
        : CProperty(name)
    {}

};

class CBoolProperty : public CProperty
{
    using TIPropertyAccessor = TPropertyAccessor<bool>;
public:
    CBoolProperty(const std::string& name)
        : CProperty(name)
    {
    }

    virtual void SetBool(void* ClassPtr, bool Value) const override
    {
        TIPropertyAccessor::Set(GetRowPtr(ClassPtr), (bool)Value);
    }
    virtual bool GetBool(void const* ClassPtr) const override
    {
        TIPropertyAccessor::Get(GetRowPtr(ClassPtr));
    }
    virtual std::string GetBoolToString(void const* ClassPtr) const 
    { 
        return GetBool(ClassPtr) ? (CStaticString::True) : (CStaticString::False);
    }
};


using CSInt8Property = TNumericProperty<int8_t>;
using CSInt16Property = TNumericProperty<int16_t>;
using CSInt32Property = TNumericProperty<int32_t>;
using CSInt64Property = TNumericProperty<int64_t>;
using CUInt8Property = TNumericProperty<int8_t>;
using CUInt16Property = TNumericProperty<int16_t>;
using CUInt32Property = TNumericProperty<int32_t>;
using CUInt64Property = TNumericProperty<int64_t>;

using CFloatProperty = TNumericProperty<float>;
using CDoubleProperty = TNumericProperty<double>;

class CStringProperty : public CProperty
{
    using TIPropertyAccessor = TPropertyAccessor<std::string>;
public:
    CStringProperty(const std::string& name)
        : CProperty(name)
    {}

    virtual std::string GetString(void const* ClassPtr) const override
    {
        return TIPropertyAccessor::Get(GetRowPtr(ClassPtr));
    }

    virtual const char* GetCString(void const* ClassPtr) const override
    {
        return TIPropertyAccessor::Get(GetRowPtr(ClassPtr)).c_str();
    }

    virtual void SetString(void* ClassPtr, const std::string& Value) const override
    {
        TIPropertyAccessor::Set(GetRowPtr(ClassPtr), Value);
    }

    virtual void SetString(void* ClassPtr, const char* Value) const override
    {
        TIPropertyAccessor::Set(GetRowPtr(ClassPtr), Value);
    }

    virtual void SetString(void* ClassPtr, uint64_t Value) const override
    {
        TIPropertyAccessor::Set(GetRowPtr(ClassPtr), std::to_string(Value));
    }

    virtual void SetString(void* ClassPtr, int64_t Value) const override
    {
        TIPropertyAccessor::Set(GetRowPtr(ClassPtr), std::to_string(Value));
    }

    virtual void SetString(void* ClassPtr, double Value) const override
    {
        TIPropertyAccessor::Set(GetRowPtr(ClassPtr), std::to_string(Value));
    }

};

class CClassProperty : public CProperty
{
public:
    CClassProperty(const std::string& name)
        : CProperty(name)
    {}
    virtual CType* GetType() override { return Class_; }
    virtual CClass* GetClass() override { return Class_; }
    void SetClass(CClass* Class) { Class_ = Class;};
    CClass* Class_;
};

class CEnumProperty : public CProperty
{
public:
    CEnumProperty(const std::string& name)
        : CProperty(name)
    {}
    virtual CType* GetType() override { return Enum_; }
    virtual CEnum* GetEnum() override { return Enum_; }
    void SetEnum(CEnum* Enum) { Enum_ = Enum; };
    CEnum* Enum_;
};

class CVectorProperty : public CProperty
{
public:
    CVectorProperty(const std::string& name)
        : CProperty(name)
    {}

    virtual CProperty* GetDataProperty() { return DataProperty_; }
    void SetDataProperty(CProperty* DataProperty) { DataProperty_ = DataProperty;}

    CProperty* DataProperty_;
};

class CMapProperty : public CProperty
{
public:
    CMapProperty(const std::string& name)
        : CProperty(name)
    {}

    CProperty* KeyProperty;
    CProperty* ValueProperty;
};

class CUnorderedMapProperty : public CProperty
{
public:
    CUnorderedMapProperty(const std::string& name)
        : CProperty(name)
    {}

    CProperty* KeyProperty;
    CProperty* ValueProperty;
};

class CUnknowProperty : public CProperty
{
public:
    CUnknowProperty(const std::string& name)
        : CProperty(name)
    {}

};


//
//template<typename T>
//class TCppField : public CMetaValue
//{
//public:
//    TField(const std::string& name, void* ptr)
//        : CMetaValue(name, ptr, sizeof(T))
//    {}
//
//};
