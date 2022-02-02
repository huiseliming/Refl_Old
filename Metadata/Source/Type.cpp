#include "Type.h"
#include <cassert>

std::unordered_map<std::string, CType*>& CType::NameToType = CType::StaticTable();

std::unordered_map<std::string, CType*>& CType::StaticTable()
{
    static std::unordered_map<std::string, CType*> Table;
    return Table;
}

std::list<std::function<void()>>& CType::PostStaticInitializerList()
{
    static std::list<std::function<void()>> StaticInitializerList;
    return StaticInitializerList;
}

void CType::PostStaticInitializer()
{
    auto& PostStaticInitializerListRef = PostStaticInitializerList();
    auto ListIt = PostStaticInitializerListRef.begin();
    while (ListIt != PostStaticInitializerListRef.end())
    {
        (*ListIt)();
        ListIt++;
    }
    PostStaticInitializerListRef.clear();
}

//CTypeManager::CTypeManager()
//{
//#define STATIC_TYPE(A, B) static CCppType A##Type(#B, sizeof(B))
//    static CCppType Type("void");
//    STATIC_TYPE(bool, bool);
//    STATIC_TYPE(unsigned_char, unsigned char);
//    STATIC_TYPE(unsigned_short, unsigned short);
//    STATIC_TYPE(unsigned_int, unsigned int);
//    STATIC_TYPE(unsigned_long, unsigned long);
//    STATIC_TYPE(unsigned_long_long, unsigned long long);
//    //STATIC_TYPE(unsigned___int128, unsigned __int128);
//    STATIC_TYPE(signed_char, signed char);
//    STATIC_TYPE(short, short);
//    STATIC_TYPE(int, int);
//    STATIC_TYPE(long, long);
//    STATIC_TYPE(long_long, long long);
//    //STATIC_TYPE(__int128, __int128);
//    STATIC_TYPE(float, float);
//    STATIC_TYPE(double, double);
//    STATIC_TYPE(long_double, long double);
//    //STATIC_TYPE(__float128, __float128);
//    STATIC_TYPE(char, char);
//    STATIC_TYPE(wchar_t, wchar_t);
//    STATIC_TYPE(char16_t, char16_t);
//    STATIC_TYPE(char32_t, char32_t);
//    STATIC_TYPE(nullptr, nullptr);
//#undef STATIC_TYPE
//}
//
//void CType::CManager::RegisterType(CType* Type)
//{
//    assert(!Metadatas.contains(Type->Name));
//    Metadatas[Type->Name] = Type;
//}
//
//CTypeManager& CTypeManager::Instance()
//{
//    static CTypeManager Mgr;
//    return Mgr;
//}
