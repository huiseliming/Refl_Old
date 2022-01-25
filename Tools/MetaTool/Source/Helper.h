#pragma once 
#include <string>
#include <cppast/cpp_type.hpp>
#include "Type.h"

const std::string& ToString(cppast::cpp_builtin_type_kind BuiltinTypeKind);

//CType* Convert(const cppast::cpp_type& InCppType);

std::string LoadStringFromFile(const std::string& InFile);




