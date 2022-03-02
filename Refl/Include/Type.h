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
    uint32_t GetSize() const { return Size_; }

    static CType* FindType(const std::string& Name) { return NameToType.contains(Name) ? NameToType[Name] : nullptr; }
    static std::unordered_map<std::string, CType*>& StaticTable();
    static void RegisterBuiltinType();
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

class REFL_API CReflTypeContainer
{
public:
    CReflTypeContainer()
        : Type_(nullptr)
        , DataPtr_(nullptr)
        , RequestFree_(true)
    {}
    CReflTypeContainer(CType* Type, void* DataPtr = nullptr, bool RequestFree = true)
    {
        Set(Type, DataPtr, RequestFree);
    }
    ~CReflTypeContainer()
    {
        Clear();
    }
    CReflTypeContainer(const CReflTypeContainer& Other)
    {
        Type_ = Other.Type_;
        DataPtr_ = Type_->New();
        Type_->CopyAssign(DataPtr_, Other.DataPtr_);
        RequestFree_ = true;
    }
    CReflTypeContainer(CReflTypeContainer&& Other)
    {
        Type_ = Other.Type_;
        DataPtr_ = Type_->New();
        Type_->MoveAssign(DataPtr_, Other.DataPtr_);
        RequestFree_ = true;
    }
    CReflTypeContainer& operator=(const CReflTypeContainer& Other)
    {
        assert(std::addressof(Other) != this);
        if (Type_ != Other.Type_)
        {
            Clear();
            Type_ = Other.Type_;
            DataPtr_ = Type_->New();
            RequestFree_ = true;
        }
        Type_->CopyAssign(DataPtr_, Other.DataPtr_);
        return *this;
    }
    CReflTypeContainer& operator=(CReflTypeContainer&& Other)
    {
        assert(std::addressof(Other) != this);
        if (Type_ != Other.Type_)
        {
            Clear();
            Type_ = Other.Type_;
            DataPtr_ = Type_->New();
            RequestFree_ = true;
        }
        Type_->MoveAssign(DataPtr_, Other.DataPtr_);
        return *this;
    }

    void Set(CType* Type, void* DataPtr = nullptr, bool RequestFree = true)
    {
        Clear();
        assert(Type);
        Type_ = Type;
        if (DataPtr)
        {
            DataPtr_ = DataPtr;
        }
        else
        {
            assert(RequestFree);
            DataPtr_ = Type_->New();
        }
        RequestFree_ = RequestFree;
    }

    void Clear()
    {
        if (DataPtr_ && RequestFree_)
        {
            assert(Type_);
            Type_->Delete(DataPtr_);
        }
        Type_ = nullptr;
        DataPtr_ = nullptr;
    }

    bool IsValid() { return DataPtr_; }

    CType* GetType() const { return Type_; }
    void* GetDataPtr() const { return DataPtr_; }

protected:
    CType* Type_;
    void* DataPtr_;
    bool RequestFree_;
};

template<typename T>
struct TEnum
{
    static CEnum* StaticEnum();
    static CEnum* ENUM_STATIC_INITIALIZER();
};

template<typename T>
class TBuiltinType : public CType
{
public:
    TBuiltinType(const std::string& InName)
        : CType(InName)
    {
    }
    static CType* BUILTIN_TYPE_STATIC_INITIALIZER();
    static CType* StaticType();
};

template<typename T>
class TNamedType : public CType
{
public:
    TNamedType(const std::string& InName)
        : CType(InName)
    {
    }

    static CType* NAMED_TYPE_STATIC_INITIALIZER();
    static CType* StaticType();
};

template<typename T>
void InitializeType(CType* Type)
{
    Type->SetSize(sizeof(T));
    if constexpr (std::is_default_constructible_v<T>)
    {
        Type->New = []() -> void* { return new T; };
        Type->Constructor = [](void* A) { new (A)T; };
    }
    Type->Delete = [](void* A) { delete (T*)A; };
    Type->Destructor = [](void* A) { ((const T*)(A))->~T(); };
    if constexpr (std::is_copy_assignable_v<T>)
        Type->CopyAssign = [](void* A, void* B) { (*(T*)A) = (*(const T*)B); };
    if constexpr (std::is_move_assignable_v<T>)
        Type->MoveAssign = [](void* A, void* B) { (*(T*)A) = std::move(*(T*)B); };
}

template<typename T>
struct TAutoInitializer
{
    TAutoInitializer()
    {
        CType* Type = nullptr;
        if constexpr (std::is_enum_v<T>) Type = TEnum<T>::StaticEnum();
        else if constexpr (std::is_arithmetic_v<T>) Type = TBuiltinType<T>::StaticType();
        else  Type = T::StaticClass();
        InitializeType<T>(Type);
        assert(!CType::StaticTable().contains(Type->GetName()));
        CType::StaticTable().insert(std::make_pair(Type->GetName(), Type));
    }
};

class CVectorTemplateType : public CType
{
public:
    CVectorTemplateType(const std::string& InName = "")
        : CType(InName)
    {}

    virtual void PushBack(void* VectorPtr, void* ValuePtr) = 0;
    virtual void PopBack(void* VectorPtr) = 0;
    virtual void Resize(void* VectorPtr, uint32_t Size) = 0;
    virtual void Remove(void* VectorPtr, uint32_t Offset, uint32_t Count) = 0;
    virtual uint32_t GetSize(void* VectorPtr) = 0;
    virtual void* GetDataPtr(void* VectorPtr, uint32_t Index = 0) = 0;

};

// vector type
template<typename T>
class TVectorTemplateType : public CVectorTemplateType
{
public:
    TVectorTemplateType(const std::string& InName = "")
        : CVectorTemplateType(InName)
    {}

    virtual void PushBack(void* VectorPtr, void* ValuePtr) override
    {
        std::vector<T>* VP = reinterpret_cast<std::vector<T>*>(VectorPtr);
        VP->push_back(*(T*)ValuePtr);
    }
    virtual void PopBack(void* VectorPtr) override
    {
        std::vector<T>* VP = reinterpret_cast<std::vector<T>*>(VectorPtr);
        VP->pop_back();
    }
    virtual void Resize(void* VectorPtr, uint32_t Size) override
    {
        std::vector<T>* VP = reinterpret_cast<std::vector<T>*>(VectorPtr);
        VP->resize(Size);
    }
    virtual void Remove(void* VectorPtr, uint32_t Offset, uint32_t Count) override
    {
        std::vector<T>* VP = reinterpret_cast<std::vector<T>*>(VectorPtr);
        VP->erase(VP->begin() + Offset, VP->begin() + Offset + Count);
    }
    virtual uint32_t GetSize(void* VectorPtr)
    {
        std::vector<T>* VP = reinterpret_cast<std::vector<T>*>(VectorPtr);
        return static_cast<uint32_t>(VP->size());
    }
    virtual void* GetDataPtr(void* VectorPtr, uint32_t Index) override
    {
        std::vector<T>* VP = reinterpret_cast<std::vector<T>*>(VectorPtr);
        return &((*VP)[Index]);
    }
};

using CTypeInfoWrapper = std::reference_wrapper<const std::type_info>;
struct Hasher {
    std::size_t operator()(CTypeInfoWrapper code) const { return code.get().hash_code(); }
};
struct EqualTo {
    bool operator()(CTypeInfoWrapper lhs, CTypeInfoWrapper rhs) const { return lhs.get() == rhs.get(); }
};

REFL_API std::unordered_map<CTypeInfoWrapper, std::unique_ptr<CVectorTemplateType>, Hasher, EqualTo>& GetTypeInfoToVectorTemplateInstantiationType();

template<typename T>
CVectorTemplateType* GetVectorTemplateInstantiationType()
{
    std::unordered_map<CTypeInfoWrapper, std::unique_ptr<CVectorTemplateType>, Hasher, EqualTo>& TypeInfoMap = GetTypeInfoToVectorTemplateInstantiationType();
    if (TypeInfoMap[typeid(T)]) return TypeInfoMap[typeid(T)].get();
    TypeInfoMap[typeid(T)].reset(new TVectorTemplateType<T>(""));
    InitializeType<T>(TypeInfoMap[typeid(T)].get());
    return TypeInfoMap[typeid(T)].get();
}
