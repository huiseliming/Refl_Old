#include "Helper.h"
#include <memory>
#include <fstream>
#include "Property.h"

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
		return CStaticString::Empty;
	}
#undef TO_STRING
}

#define CONST_STRING(A) static std::string A##String = #A
CONST_STRING(CVoidProperty);
CONST_STRING(CBoolProperty);
CONST_STRING(CUInt8Property);
CONST_STRING(CUInt16Property);
CONST_STRING(CUInt32Property);
CONST_STRING(CUInt64Property);
CONST_STRING(CSInt8Property);
CONST_STRING(CSInt16Property);
CONST_STRING(CSInt32Property);
CONST_STRING(CSInt64Property);
CONST_STRING(CFloatProperty);
CONST_STRING(CDoubleProperty);
CONST_STRING(CStringProperty);
CONST_STRING(CClassProperty);
CONST_STRING(CEnumProperty);
CONST_STRING(CVectorProperty);
CONST_STRING(CMapProperty);
CONST_STRING(CUnorderedMapProperty);
CONST_STRING(CUnknowProperty);
#undef CONST_STRING

const std::string& ToPropertyTypeName(uint64_t PropertyFlag)
{
#define TO_STRING(X) case cppast::##X: return X

	switch (PropertyFlag & EPF_TypeFlagBits)
	{
	case EPF_ZeroFlag         : return CUnknowPropertyString;
	case EPF_VoidFlag         : return CVoidPropertyString; 
	case EPF_BoolFlag         : return CBoolPropertyString;
	case EPF_SInt8Flag        : return CSInt8PropertyString;
	case EPF_SInt16Flag       : return CSInt16PropertyString;
	case EPF_SInt32Flag       : return CSInt32PropertyString;
	case EPF_SInt64Flag       : return CSInt64PropertyString;
	case EPF_UInt8Flag        : return CUInt8PropertyString;
	case EPF_UInt16Flag       : return CUInt16PropertyString;
	case EPF_UInt32Flag       : return CUInt32PropertyString;
	case EPF_UInt64Flag       : return CUInt64PropertyString;
	case EPF_FloatFlag        : return CFloatPropertyString;
	case EPF_DoubleFlag       : return CDoublePropertyString;
	case EPF_StringFlag       : return CStringPropertyString;
	case EPF_ClassFlag        : return CClassPropertyString;
	case EPF_EnumFlag	      : return CEnumPropertyString;
	case EPF_VectorFlag       : return CVectorPropertyString;
	case EPF_MapFlag          : return CMapPropertyString;
	case EPF_UnorderedMapFlag : return CUnorderedMapPropertyString;
	default:
		break;
	}
	return CUnknowPropertyString;
}

uint64_t MakeBuiltinTypeFlags(cppast::cpp_builtin_type_kind BuiltinTypeKind)
{
	switch (BuiltinTypeKind)
	{
	case cppast::cpp_builtin_type_kind::cpp_void:       return EPF_VoidFlag;
	case cppast::cpp_builtin_type_kind::cpp_bool:       return EPF_BoolFlag;
	case cppast::cpp_builtin_type_kind::cpp_uchar:      return EPF_SInt8Flag;
	case cppast::cpp_builtin_type_kind::cpp_ushort:     return EPF_SInt16Flag;
	case cppast::cpp_builtin_type_kind::cpp_uint:       return EPF_SInt32Flag;
	case cppast::cpp_builtin_type_kind::cpp_ulong:      return sizeof(unsigned long) == 4 ? EPF_UInt32Flag : EPF_UInt64Flag;
	case cppast::cpp_builtin_type_kind::cpp_ulonglong:  return EPF_SInt64Flag;
	case cppast::cpp_builtin_type_kind::cpp_uint128:    return 0;
	case cppast::cpp_builtin_type_kind::cpp_schar:      return EPF_UInt8Flag;
	case cppast::cpp_builtin_type_kind::cpp_short:      return EPF_UInt16Flag;
	case cppast::cpp_builtin_type_kind::cpp_int:        return EPF_UInt32Flag;
	case cppast::cpp_builtin_type_kind::cpp_long:       return sizeof(long) == 4 ? EPF_SInt32Flag : EPF_SInt64Flag;
	case cppast::cpp_builtin_type_kind::cpp_longlong:   return EPF_UInt64Flag;
	case cppast::cpp_builtin_type_kind::cpp_int128:     return 0;
	case cppast::cpp_builtin_type_kind::cpp_float:      return EPF_FloatFlag;
	case cppast::cpp_builtin_type_kind::cpp_double:     return EPF_DoubleFlag;
	case cppast::cpp_builtin_type_kind::cpp_longdouble: return 0;
	case cppast::cpp_builtin_type_kind::cpp_float128:   return 0;
	case cppast::cpp_builtin_type_kind::cpp_char:       return EPF_SInt8Flag;
	case cppast::cpp_builtin_type_kind::cpp_wchar:      return 0;
	case cppast::cpp_builtin_type_kind::cpp_char16:     return 0;
	case cppast::cpp_builtin_type_kind::cpp_char32:     return 0;
	case cppast::cpp_builtin_type_kind::cpp_nullptr:    return 0;
	default:
		return 0;
	}
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

std::string GetOuputHeaderFileFullPath(std::string InputFileFullPath)
{
	size_t Pos = InputFileFullPath.rfind(".");
	if (Pos == std::string::npos) return "";
	return InputFileFullPath.substr(0, Pos) + ".generated.h";
}

std::string GetOuputSourceFileFullPath(std::string InputFileFullPath)
{
	size_t Pos = InputFileFullPath.rfind(".");
	if (Pos == std::string::npos) return "";
	return InputFileFullPath.substr(0, Pos) + ".generated.cpp";
}

