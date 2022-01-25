#include "Helper.h"
#include <memory>
#include <fstream>

#define CONST_STRING(A, B) static std::string A = #B

CONST_STRING(cpp_void, void);
CONST_STRING(cpp_bool, bool);
CONST_STRING(cpp_uchar, unsigned char);
CONST_STRING(cpp_ushort, unsigned short);
CONST_STRING(cpp_uint, unsigned int);
CONST_STRING(cpp_ulong, unsigned long);
CONST_STRING(cpp_ulonglong, unsigned long long);
CONST_STRING(cpp_uint128, unsigned __int128);
CONST_STRING(cpp_schar, signed char);
CONST_STRING(cpp_short, short);
CONST_STRING(cpp_int, int);
CONST_STRING(cpp_long, long);
CONST_STRING(cpp_longlong, long long);
CONST_STRING(cpp_int128, __int128);
CONST_STRING(cpp_float, float);
CONST_STRING(cpp_double, double);
CONST_STRING(cpp_longdouble, long double);
CONST_STRING(cpp_float128, __float128);
CONST_STRING(cpp_char, char);
CONST_STRING(cpp_wchar, wchar_t);
CONST_STRING(cpp_char16, char16_t);
CONST_STRING(cpp_char32, char32_t);
CONST_STRING(cpp_nullptr, nullptr);

#undef CONST_STRING

const std::string& ToString(cppast::cpp_builtin_type_kind BuiltinTypeKind)
{
#define TO_STRING(X) case cppast::##X: return X

	switch (BuiltinTypeKind)
	{
	TO_STRING(cpp_void);
	TO_STRING(cpp_bool);
	TO_STRING(cpp_uchar);
	TO_STRING(cpp_ushort);
	TO_STRING(cpp_uint);
	TO_STRING(cpp_ulong);
	TO_STRING(cpp_ulonglong);
	TO_STRING(cpp_uint128);
	TO_STRING(cpp_schar);
	TO_STRING(cpp_short);
	TO_STRING(cpp_int);
	TO_STRING(cpp_long);
	TO_STRING(cpp_longlong);
	TO_STRING(cpp_int128);
	TO_STRING(cpp_float);
	TO_STRING(cpp_double);
	TO_STRING(cpp_longdouble);
	TO_STRING(cpp_float128);
	TO_STRING(cpp_char);
	TO_STRING(cpp_wchar);
	TO_STRING(cpp_char16);
	TO_STRING(cpp_char32);
	TO_STRING(cpp_nullptr);
	default:
		return "";
	}
#undef TO_STRING
}

//CType* Convert(const cppast::cpp_type& InCppType)
//{
//	std::shared_ptr<CType> QualifiedType;
//	while (true)
//	{
//		if (InCppType.kind() == cppast::cpp_type_kind::reference_t)
//		{
//			const auto& RefType = static_cast<const cppast::cpp_reference_type&>(InCppType);
//		}
//		if (InCppType.kind() == cppast::cpp_type_kind::pointer_t)
//		{
//			const auto& PtrType = static_cast<const cppast::cpp_pointer_type&>(InCppType);
//		}
//		if (InCppType.kind() == cppast::cpp_type_kind::cv_qualified_t)
//		{
//			const auto& CVType = static_cast<const cppast::cpp_cv_qualified_type&>(InCppType);
//		}
//	}
//	return nullptr;
//}

std::string LoadStringFromFile(const std::string& InFile)
{
	std::string Output;
	std::ifstream IfStream(InFile);
	if (!IfStream) return Output;
	IfStream.seekg(0, std::ios::end);
	Output.reserve(static_cast<std::string::size_type>(IfStream.tellg()));
	IfStream.seekg(0, std::ios::beg);
	Output.assign((std::istreambuf_iterator<char>(IfStream)), std::istreambuf_iterator<char>());
	IfStream.close();
	return Output;
}
