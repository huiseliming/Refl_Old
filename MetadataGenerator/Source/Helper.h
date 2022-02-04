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
//GetLastWriteTime