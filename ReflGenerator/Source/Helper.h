#pragma once 
#include <string>
#include <cppast/cpp_type.hpp>
#include "Type.h"
#include "mustache.hpp"

const std::string& ToString(cppast::cpp_builtin_type_kind BuiltinTypeKind);
const std::string& ToPropertyTypeName(uint64_t PropertyFlag);
uint64_t MakeBuiltinTypeFlags(cppast::cpp_builtin_type_kind BuiltinTypeKind);

//CType* Convert(const cppast::cpp_type& InCppType);

std::string LoadStringFromFile(const std::string& InFile);

std::string GetOuputHeaderFileFullPath(std::string InputFileFullPath);
std::string GetOuputSourceFileFullPath(std::string InputFileFullPath);

std::string FormatCustomMetadata(const std::string& CustomMetadata);

kainjow::mustache::data MakeTmplMetadataKVList(std::unordered_map<std::string, std::string> MetadataMap);


std::string ParseMemberVariableCppTypeToPropertyStaticInitializerCode(
    const cppast::cpp_entity_index& EntityIndex, 
    const cppast::cpp_type& Type, 
    const std::string& ClassName, 
    const std::string& PropertyName,
    const std::unordered_map<std::string, std::string>& PropertyMetadatas);

std::string ParseMemberFunctionCppTypeToPropertyStaticInitializerCode(
    const cppast::cpp_entity_index& EntityIndex,
    const cppast::cpp_type& Type,
    const std::string& ClassName,
    const std::string& PropertyName,
    const std::unordered_map<std::string, std::string>& PropertyMetadatas);

std::string ParseCppTypeToPropertyStaticInitializerCode(
    const cppast::cpp_entity_index& EntityIndex,
    const cppast::cpp_type& Type,
    const std::string& ClassName,
    const std::string& PropertyName,
    const std::unordered_map<std::string, std::string>& PropertyMetadatas);

std::string GeneratePropertyStaticInitializerFunctionCode(
    const std::string& StaticInitializerFunctionName, 
    const std::string& PropertyName, 
    uint64_t PropertyFlag, 
    const std::string& PropertyAddressOffset,
    const std::unordered_map<std::string, std::string>& PropertyMetadatas,
    const std::string& PropertyClassName = {},
    const std::string& PropertyEnumName = {},
    const std::string& VectorSubPropertyFunctionName = {},
    const std::string& VectorTemplateInstantiationTypeName = {}
);

std::string ParseCppTypeToSpellString(const cppast::cpp_type& CppType, bool bIsRemoveCV = true);

bool IsRefType(const cppast::cpp_type& CppType);

//GetLastWriteTime