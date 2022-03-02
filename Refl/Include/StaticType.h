#pragma once
#include "Type.h"
#include "Enum.h"
#include "Class.h"





//template<typename T>
//CType* StaticType()
//{
//    static 
//    std::conditional_t
//    <
//        std::is_class_v<T>,
//        CClass, 
//        std::conditional_t
//        <
//            std::is_enum_v<T>, 
//            CEnum,
//            CType
//        >
//    > Type("");
//    return &Type;
//}
//
//template<typename T>
//CEnum* StaticEnum()
//{
//    return reinterpret_cast<CEnum*>(StaticType<T>());
//}
//
//template<typename T>
//CClass* StaticClass()
//{
//    return reinterpret_cast<CClass*>(StaticType<T>());
//}

struct REFL_API CStaticString
{
    static const std::string Empty;
    static const std::string True;
    static const std::string False;
};
