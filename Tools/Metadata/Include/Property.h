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

    CType*   GetType()                { return Type; }
    uint64_t GetFlag()                { return Flag; }
    void*    GetRowPtr(void* ClassBasePtr) { return (void*)(((char*)(ClassBasePtr)) + AddressOffset); }

    virtual bool GetBool(void const* ClassBasePtr) const { return false; }
    virtual void SetBool(void* ClassBasePtr, bool Value) const {}

    virtual int64_t GetSInt(void const* ClassBasePtr) const { return 0; }
    virtual uint64_t GetUInt(void const* ClassBasePtr) const { return 0; }
    virtual double GetFloat(void const* ClassBasePtr) const { return 0.f; }
    virtual void SetUInt(void* ClassBasePtr, int64_t Value) const {}
    virtual void SetSInt(void* ClassBasePtr, uint64_t Value) const {}
    virtual void SetFloat(void* ClassBasePtr, double Value) const {}

    virtual std::string GetString(void const* ClassBasePtr) const { return ""; }
    virtual const char* GetCString(void const* ClassBasePtr) const { return ""; }
    virtual void SetString(void* ClassBasePtr, int64_t Value) const {}
    virtual void SetString(void* ClassBasePtr, uint64_t Value) const {}
    virtual void SetString(void* ClassBasePtr, double Value) const {}
    virtual void SetString(void* ClassBasePtr, const char* Value) const {}
    virtual void SetString(void* ClassBasePtr, const std::string& Value) const {}

    virtual void SetNumericFromString(void* Data, char const* Value) const {}
    virtual std::string GetNumericToString(void const* Data) const { return ""; }
    virtual std::string GetBoolToString(void const* Data) const { return ""; }

protected:
    CType* Type;
    uint64_t Flag;
    uint32_t AddressOffset;
};

template<typename T>
struct TPropertyAccessor
{
    enum
    {
        CppSize = sizeof(T),
        CppAlignment = alignof(T)
    };
    static T const* GetPtr(void const* A) 
    { 
        return (T const*)(A); 
    }
    static T* GetPtr(void* A)
    {
        return (T*)(A);
    }
    static T const& Get(void const* A)
    {
        return *GetPtr();
    }
    static T GetDefault()
    {
        return T();
    }
    static T GetOptional(void const* B)
    {
        return B ? Get(B) : GetDefault();
    }
    static void Set(void* A, T const& Value)
    {
        *GetPtr(A) = Value;
    }
    static T* Initialize(void* A)
    {
        return new (A) T();
    }
    static void Destroy(void* A)
    {
        GetPtr(A)->~T();
    }
};

template <typename T>
struct TNumericProperty : public CProperty
{
    using TIPropertyAccessor = TPropertyAccessor<T>;

    TNumericProperty(const std::string& name)
        : CProperty(name)
    {
    }

    virtual void SetUInt(void* ClassBasePtr, uint64_t Value) const override
    {
        assert(std::is_integral_v<T>);
        TIPropertyAccessor::Set(GetRowPtr(ClassBasePtr), (T)Value);
    }
    virtual void SetSInt(void* ClassBasePtr, int64_t Value) const override
    {
        assert(std::is_integral_v<T>);
        TIPropertyAccessor::Set(GetRowPtr(ClassBasePtr), (T)Value);
    }
    virtual void SetFloat(void* ClassBasePtr, double Value) const override
    {
        assert(std::is_floating_point_v<T>);
        TIPropertyAccessor::Set(GetRowPtr(ClassBasePtr), (T)Value);
    }
    virtual void SetNumericFromString(void* ClassBasePtr, char const* Value) const override
    {
//#pragma warning(push)
//#pragma warning (disable: 4244)
        *TIPropertyAccessor::GetPtr(GetRowPtr(ClassBasePtr)) = (T)atoll(Value);
//#pragma warning(pop)
    }
    virtual std::string GeNumericToString(void const* ClassBasePtr) const override
    {
        return std::to_string(TIPropertyAccessor::Get(GetRowPtr(ClassBasePtr)));
    }
    virtual int64_t GetSInt(void const* ClassBasePtr) const override
    {
        assert(std::is_integral_v<T>);
        return (int64_t)TIPropertyAccessor::Get(GetRowPtr(ClassBasePtr));
    }
    virtual uint64_t GetUInt(void const* ClassBasePtr) const override
    {
        assert(std::is_integral_v<T>);
        return (uint64_t)TIPropertyAccessor::Get(GetRowPtr(ClassBasePtr));
    }
    virtual double GetFloat(void const* ClassBasePtr) const override
    {
        assert(std::is_floating_point_v<T>);
        return (double)TIPropertyAccessor::Get(GetRowPtr(ClassBasePtr));
    }

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
