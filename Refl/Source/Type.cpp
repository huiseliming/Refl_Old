#include "Type.h"
#include "Archive.h"
#include <cassert>

std::unordered_map<std::string, CType*>& CType::NameToType = CType::StaticTable();

std::unordered_map<std::string, CType*>& CType::StaticTable()
{
    static std::unordered_map<std::string, CType*> Table;
    return Table;
}

std::list<std::function<void()>>& CType::PostStaticInitializerEventList()
{
    static std::list<std::function<void()>> StaticInitializerList;
    return StaticInitializerList;
}

void CType::ProcessPostStaticInitializerEvent()
{
    auto& PostStaticInitializerListRef = PostStaticInitializerEventList();
    auto ListIt = PostStaticInitializerListRef.begin();
    while (ListIt != PostStaticInitializerListRef.end())
    {
        (*ListIt)();
        ListIt++;
    }
    PostStaticInitializerListRef.clear();
}

#define DECL_BUILTIN_TYPE_STATIC_TYPE(BuiltinType,NameString)\
template<>\
CType* TBuiltinType<BuiltinType>::BUILTIN_TYPE_STATIC_INITIALIZER()\
{\
    static TBuiltinType<BuiltinType> ST(NameString); \
    return &ST; \
}\
template<>\
CType* TBuiltinType<BuiltinType>::StaticType() \
{\
    static CType* T = BUILTIN_TYPE_STATIC_INITIALIZER();\
    return T;\
}\

DECL_BUILTIN_TYPE_STATIC_TYPE(bool,     "Bool")
DECL_BUILTIN_TYPE_STATIC_TYPE(uint8_t,  "UInt8")
DECL_BUILTIN_TYPE_STATIC_TYPE(uint16_t, "UInt16")
DECL_BUILTIN_TYPE_STATIC_TYPE(uint32_t, "UInt32")
DECL_BUILTIN_TYPE_STATIC_TYPE(uint64_t, "UInt64")
DECL_BUILTIN_TYPE_STATIC_TYPE(int8_t,   "SInt8")
DECL_BUILTIN_TYPE_STATIC_TYPE(int16_t,  "SInt16")
DECL_BUILTIN_TYPE_STATIC_TYPE(int32_t,  "SInt32")
DECL_BUILTIN_TYPE_STATIC_TYPE(int64_t,  "SInt64")
DECL_BUILTIN_TYPE_STATIC_TYPE(float,    "Float")
DECL_BUILTIN_TYPE_STATIC_TYPE(double,   "Double")

#undef DECL_BUILTIN_TYPE_STATIC_TYPE

template class REFL_API TBuiltinType<bool>;
template class REFL_API TBuiltinType<uint8_t>;
template class REFL_API TBuiltinType<uint16_t>;
template class REFL_API TBuiltinType<uint32_t>;
template class REFL_API TBuiltinType<uint64_t>;
template class REFL_API TBuiltinType<int8_t>;
template class REFL_API TBuiltinType<int16_t>;
template class REFL_API TBuiltinType<int32_t>;
template class REFL_API TBuiltinType<int64_t>;
template class REFL_API TBuiltinType<float>;
template class REFL_API TBuiltinType<double>;

static TAutoInitializer<bool>     BoolAutoInitializer;
static TAutoInitializer<uint8_t>  UInt8AutoInitializer;
static TAutoInitializer<uint16_t> UInt16AutoInitializer;
static TAutoInitializer<uint32_t> UInt32AutoInitializer;
static TAutoInitializer<uint64_t> UInt64AutoInitializer;
static TAutoInitializer<int8_t>   SInt8AutoInitializer;
static TAutoInitializer<int16_t>  SInt16AutoInitializer;
static TAutoInitializer<int32_t>  SInt32AutoInitializer;
static TAutoInitializer<int64_t>  SInt64AutoInitializer;
static TAutoInitializer<float>    FloatAutoInitializer;
static TAutoInitializer<double>   DoubleAutoInitializer;

template<>
CType* TNamedType<std::string>::NAMED_TYPE_STATIC_INITIALIZER()
{
    static CType T("String");
    return &T;
}

template<>
CType* TNamedType<std::string>::StaticType()
{
    static CType* T = NAMED_TYPE_STATIC_INITIALIZER();
    return T;
}

template class REFL_API TNamedType<std::string>;

static std::function<CType* ()> StringAutoInitializer = 
[] () -> CType*{
    CType* StringType = TNamedType<std::string>::StaticType();
    InitializeType<std::string>(StringType);
    assert(!CType::StaticTable().contains(StringType->GetName()));
    CType::StaticTable().insert(std::make_pair(StringType->GetName(), StringType));
    return StringType;
};

static CType* StringType = StringAutoInitializer();

//
//template<>
//CType* TAnonymousType<std::vector<int>>::ANONYMOUS_TYPE_STATIC_INITIALIZER()
//{
//    static TAnonymousType<std::vector<int>> ST();
//    InitializeType<std::vector<int>>(&ST);
//    return &ST; 
//}
//
//template<>
//CType* TAnonymousType<std::vector<int>>::StaticType()
//{
//    static CType* T = ANONYMOUS_TYPE_STATIC_INITIALIZER();
//    return T; 
//}
//template class REFL_API TAnonymousType<std::vector<int>>;
//
//static TAutoInitializer<bool> std_vector_int_AutoInitializer;
//

std::unordered_map<CTypeInfoWrapper, std::unique_ptr<CVectorTemplateType>, Hasher, EqualTo>& GetTypeInfoToVectorTemplateInstantiationType()
{
    static std::unordered_map<CTypeInfoWrapper, std::unique_ptr<CVectorTemplateType>, Hasher, EqualTo> TypeInfoMap;
    return TypeInfoMap;
}
