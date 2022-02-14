#include "Parser.h"
#include "Property.h"
#include "cppast/cpp_entity_kind.hpp"
#include "fmt/format.h"

CPropertyInfo ParseCppTypeToPropertyInfo(const cppast::cpp_entity_index& EntityIndex, const cppast::cpp_type& CppType)
{
    CPropertyInfo PropertyInfo = {};
    std::string PtrOrRefString, CVQualifiedString;
    PropertyInfo.PropertyFlag = 0;
    const cppast::cpp_type* CppTypePtr = &CppType;
    if (CppTypePtr->kind() == cppast::cpp_type_kind::pointer_t)
    {
        auto CppMemberVariablePointerType = static_cast<const cppast::cpp_pointer_type*>(CppTypePtr);
        CppTypePtr = &CppMemberVariablePointerType->pointee();
        PropertyInfo.PropertyFlag |= EPF_PointerFlag;
        PropertyInfo.Spelling += "*";
    }
    if (CppTypePtr->kind() == cppast::cpp_type_kind::reference_t)
    {
        auto CppMemberVariableReferenceType = static_cast<const cppast::cpp_reference_type*>(CppTypePtr);
        CppTypePtr = &CppMemberVariableReferenceType->referee();
        PropertyInfo.PropertyFlag |= EPF_ReferenceFlag;
        PropertyInfo.Spelling += "&";
    }
    if (CppTypePtr->kind() == cppast::cpp_type_kind::cv_qualified_t)
    {
        auto CppMemberVariableCVQualifiedType = static_cast<const cppast::cpp_cv_qualified_type*>(CppTypePtr);
        CppTypePtr = &CppMemberVariableCVQualifiedType->type();
        auto CVQualifier = CppMemberVariableCVQualifiedType->cv_qualifier();
        switch (CVQualifier)
        {
        case cppast::cpp_cv_const:
            PropertyInfo.PropertyFlag |= EPF_ConstFlag;
            PropertyInfo.Spelling = "const " + PropertyInfo.Spelling;
            break;
        case cppast::cpp_cv_volatile:
            PropertyInfo.PropertyFlag |= EPF_VolatileFlag;
            break;
        case cppast::cpp_cv_const_volatile:
            PropertyInfo.PropertyFlag |= (EPF_ConstFlag | EPF_VolatileFlag);
            break;
        case cppast::cpp_cv_none:
        default:
            break;
        }
    }
    if (CppTypePtr->kind() == cppast::cpp_type_kind::builtin_t)
    {
        auto& CppMemberVariableBuiltinType = static_cast<const cppast::cpp_builtin_type&>(*CppTypePtr);
        PropertyInfo.PropertyFlag |= MakeBuiltinTypeFlags(CppMemberVariableBuiltinType.builtin_type_kind());
        PropertyInfo.Spelling = cppast::to_string(CppMemberVariableBuiltinType.builtin_type_kind());
    }
    else if (CppTypePtr->kind() == cppast::cpp_type_kind::user_defined_t)
    {
        auto& CppMemberVariableUserDefinedType = static_cast<const cppast::cpp_user_defined_type&>(*CppTypePtr);
        std::string CppMemberVariableUserDefinedTypeName = CppMemberVariableUserDefinedType.entity().name();
        if      (CppMemberVariableUserDefinedTypeName == "uint8_t")       PropertyInfo.PropertyFlag |= EPF_UInt8Flag;
        else if (CppMemberVariableUserDefinedTypeName == "uint16_t")      PropertyInfo.PropertyFlag |= EPF_UInt16Flag;
        else if (CppMemberVariableUserDefinedTypeName == "uint32_t")      PropertyInfo.PropertyFlag |= EPF_UInt32Flag;
        else if (CppMemberVariableUserDefinedTypeName == "uint64_t")      PropertyInfo.PropertyFlag |= EPF_UInt64Flag;
        else if (CppMemberVariableUserDefinedTypeName == "int8_t")        PropertyInfo.PropertyFlag |= EPF_SInt8Flag;
        else if (CppMemberVariableUserDefinedTypeName == "int16_t")       PropertyInfo.PropertyFlag |= EPF_SInt16Flag;
        else if (CppMemberVariableUserDefinedTypeName == "int32_t")       PropertyInfo.PropertyFlag |= EPF_SInt32Flag;
        else if (CppMemberVariableUserDefinedTypeName == "int64_t")       PropertyInfo.PropertyFlag |= EPF_SInt64Flag;
        else if (CppMemberVariableUserDefinedTypeName == "std::string")   PropertyInfo.PropertyFlag |= EPF_StringFlag;
        else
        {
            if (static_cast<size_t>(CppMemberVariableUserDefinedType.entity().id().size()) > 0)
            {
                auto entity_id = CppMemberVariableUserDefinedType.entity().id().begin();
                if (entity_id)
                {
                    auto entity_opt = EntityIndex.lookup(*entity_id);
                    if (entity_opt.has_value())
                    {
                        auto& entity = entity_opt.value();
                        if (entity.kind() == cppast::cpp_entity_kind::class_t)
                        {
                            PropertyInfo.PropertyFlag |= EPF_ClassFlag;
                            PropertyInfo.PropertyClassName = entity.name();
                        }
                        else if (entity.kind() == cppast::cpp_entity_kind::enum_t)
                        {
                            PropertyInfo.PropertyFlag |= EPF_EnumFlag;
                            PropertyInfo.PropertyEnumName = entity.name();
                        }
                    }
                }
            }
        }
        PropertyInfo.Spelling = CppMemberVariableUserDefinedTypeName;
    }
    PropertyInfo.Spelling = CVQualifiedString + " " + PropertyInfo.Spelling + PtrOrRefString;
    return PropertyInfo;
}
