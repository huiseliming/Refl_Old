#pragma once
#include "Helper.h"
#include "CodeGenerator.h"


struct CPropertyInfo
{
    uint64_t PropertyFlag;
    std::string PropertyClassName;
    std::string PropertyEnumName;
    std::string Spelling;
};

CPropertyInfo ParseCppTypeToPropertyInfo(const cppast::cpp_entity_index& EntityIndex, const cppast::cpp_type& CppType);

void ParseVectorProperty(CCodeGenerator& CodeGenerator, const cppast::cpp_entity_index& EntityIndex, const cppast::cpp_template_instantiation_type& TemplateInstantiationType);

void ParseMapProperty(CCodeGenerator& CodeGenerator, const cppast::cpp_entity_index& idx, const cppast::cpp_template_instantiation_type& TemplateInstantiationType);


