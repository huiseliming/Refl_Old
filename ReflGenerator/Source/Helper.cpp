#include "Helper.h"
#include <memory>
#include <fmt/format.h>
#include <fstream>
#include "Property.h"
#include "GeneratedTemplates.h"
#include "Parser.h"
#include <cppast/cpp_template.hpp>

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

std::string FormatCustomMetadata(const std::string& CustomMetadata)
{
	std::string ReplaceCustomMetadata;
	bool IsInStringRange = false;
	bool FoundFirstEqual = false;
	for (size_t i = 0; i < CustomMetadata.size(); i++)
	{
		std::function<void()> SetNotInStringRange;
		if (CustomMetadata[i] == '\"')
		{
			if (IsInStringRange)
			{
				if (CustomMetadata[i - 1] != '\\')
				{
					SetNotInStringRange = [&] { IsInStringRange = false; };
				}
			}
			else
			{
				IsInStringRange = true;
			}
		}
		if (IsInStringRange)
		{
			ReplaceCustomMetadata.push_back(CustomMetadata[i]);
		}
		else
		{
			if (('a' <= CustomMetadata[i] && CustomMetadata[i] <= 'z') ||
				('A' <= CustomMetadata[i] && CustomMetadata[i] <= 'Z') ||
				('0' <= CustomMetadata[i] && CustomMetadata[i] <= '9') || 
				CustomMetadata[i] == '_'
			)
			{
				ReplaceCustomMetadata.push_back(CustomMetadata[i]);
			}
			if (CustomMetadata[i] == '=' && !FoundFirstEqual)
			{
				ReplaceCustomMetadata.push_back(CustomMetadata[i]);
				FoundFirstEqual = true;
			}
		}
		if (SetNotInStringRange) SetNotInStringRange();
	}
	return ReplaceCustomMetadata;
}

kainjow::mustache::data MakeTmplMetadataKVList(std::unordered_map<std::string, std::string> MetadataMap)
{
	kainjow::mustache::data MetadataKVList{ kainjow::mustache::data::type::list };
	for (auto Metadata : MetadataMap)
	{
		kainjow::mustache::data MetadataKV;
		MetadataKV.set("Key", Metadata.first);
		MetadataKV.set("Value", Metadata.second);
		MetadataKVList.push_back(MetadataKV);
	}
	return MetadataKVList;
}

std::string ParseMemberVariableCppTypeToPropertyStaticInitializerCode(const cppast::cpp_entity_index& EntityIndex, const cppast::cpp_type& Type, const std::string& ClassName, const std::string& PropertyName, const std::unordered_map<std::string, std::string>& PropertyMetadatas)
{
	return std::string();
}

std::string ParseMemberFunctionCppTypeToPropertyStaticInitializerCode(const cppast::cpp_entity_index& EntityIndex, const cppast::cpp_type& Type, const std::string& ClassName, const std::string& PropertyName, const std::unordered_map<std::string, std::string>& PropertyMetadatas)
{
	return std::string();
}

std::string ParseCppTypeToPropertyStaticInitializerCode(
	const cppast::cpp_entity_index& EntityIndex, 
	const cppast::cpp_type& Type, 
	const std::string& ClassName, 
	const std::string& PropertyName, 
	const std::unordered_map<std::string, std::string>& PropertyMetadatas)
{
	const cppast::cpp_type* TypePtr = &Type;
	std::string PropertyStaticInitializerCode;
	CPropertyInfo PropertyInfo = {};
	if (TypePtr->kind() == cppast::cpp_type_kind::template_instantiation_t)
	{
		auto& CppMemberVariableTemplateInstantiationType = static_cast<const cppast::cpp_template_instantiation_type&>(*TypePtr);
		if (CppMemberVariableTemplateInstantiationType.primary_template().name() == "std::vector")
		{
			if (CppMemberVariableTemplateInstantiationType.arguments_exposed())
			{
				auto TemplateArgment = CppMemberVariableTemplateInstantiationType.arguments();
				auto size = TemplateArgment.value().size();
				//CCodeGenerator::Instance().PropertyInitializer_.set("PropertyTypeClass", "CVectorProperty");
				auto& CppMemberVariableVectorType = CppMemberVariableTemplateInstantiationType.arguments().value().begin()->type().value();
				CPropertyInfo VectorElementPropertyInfo = ParseCppTypeToPropertyInfo(EntityIndex, CppMemberVariableVectorType);
				std::string PropertyClassName;
				std::string VectorSubPropertyFunctionName;
				if (VectorElementPropertyInfo.PropertyFlag & EPF_ClassFlag)
				{
					PropertyClassName = VectorElementPropertyInfo.PropertyClassName;
				}
				//else if (PropertyInfo.PropertyFlag & EPF_VectorFlag)
				//{
				//	VectorSubPropertyFunctionName = "CLS_" + ClassName + "__PROP_" + PropertyName + "__VectorElement__STATIC_INITIALIZER";
				//}
				PropertyStaticInitializerCode += 
					GeneratePropertyStaticInitializerFunctionCode(
						"CLS_" + ClassName + "__PROP_" + PropertyName + "__VectorElement__STATIC_INITIALIZER",
						PropertyName + "__VectorElement",
						VectorElementPropertyInfo.PropertyFlag,
						fmt::format("offsetof({}, {}::{})", ClassName, ClassName, PropertyName),
						{},
						PropertyClassName
					);
			}
		}
	}
	else
	{
		PropertyInfo = ParseCppTypeToPropertyInfo(EntityIndex, Type);
	}
	if (!(PropertyInfo.PropertyFlag & EPF_TypeFlagBits))
	{
		PropertyInfo.PropertyFlag |= EPF_UnknowFlag;
	}

	std::string PropertyClassName;
	std::string VectorSubPropertyFunctionName;
	if (PropertyInfo.PropertyFlag & EPF_ClassFlag)
	{
		PropertyClassName = PropertyInfo.PropertyClassName;
	}
	else if (PropertyInfo.PropertyFlag & EPF_VectorFlag)
	{
		VectorSubPropertyFunctionName = "CLS_" + ClassName + "__PROP_" + PropertyName + "__VectorElement__STATIC_INITIALIZER";
	}
	PropertyStaticInitializerCode +=
		GeneratePropertyStaticInitializerFunctionCode(
			"CLS_" + ClassName + "__PROP_" + PropertyName + "__STATIC_INITIALIZER",
			PropertyName,
			PropertyInfo.PropertyFlag,
			fmt::format("offsetof({}, {}::{})", ClassName, ClassName, PropertyName),
			PropertyMetadatas,
			PropertyClassName,
			VectorSubPropertyFunctionName
		);
	return PropertyStaticInitializerCode;
}

std::string GeneratePropertyStaticInitializerFunctionCode(
	const std::string& StaticInitializerFunctionName,
	const std::string& PropertyName,
	uint64_t PropertyFlag,
	const std::string& PropertyAddressOffset,
	const std::unordered_map<std::string, std::string>& PropertyMetadatas,
	const std::string& PropertyClassName,
	const std::string& VectorSubPropertyFunctionName
)
{
	kainjow::mustache::mustache PropertyInitializerFunctionTmpl(GeneratedTemplates::PropertyInitializerFunctionTemplate);
	kainjow::mustache::data PropertyInitializerFunctionData;
	kainjow::mustache::data ExpressionList{ kainjow::mustache::data::type::list };
	PropertyInitializerFunctionData.set("PropertyStaticInitializerFunctionName", StaticInitializerFunctionName);
	PropertyInitializerFunctionData.set("PropertyName", PropertyName);
	PropertyInitializerFunctionData.set("PropertyAddressOffset", PropertyAddressOffset);
	PropertyInitializerFunctionData.set("PropertyTypeClass", ToPropertyTypeName(PropertyFlag));
	PropertyInitializerFunctionData.set("PropertyFlags", fmt::format("{:#x}", PropertyFlag));
	for (auto Metadata : PropertyMetadatas)
	{
		ExpressionList.push_back(
			"    Prop.AddMetadata(\"" + Metadata.first + "\", \"" + Metadata.second + "\");\n"
		);
	}
	if (PropertyFlag & EPF_ClassFlag)
	{
		ExpressionList.push_back(
			"    CType::PostStaticInitializerEventList().push_back([&]{\n"
			"        assert(CType::NameToType.contains(\"" + PropertyClassName + "\"));\n"
			"        Prop.SetClass(reinterpret_cast<CClass*>(CType::NameToType[\"" + PropertyClassName + "\"]));\n"
			"    });\n"
		);
	}
	else if (PropertyFlag & EPF_VectorFlag)
	{
		ExpressionList.push_back(
			"    Prop.SetDataProperty(" + VectorSubPropertyFunctionName + "());\n"
		);
	}
	PropertyInitializerFunctionData.set("ExpressionList", ExpressionList);
	return PropertyInitializerFunctionTmpl.render(PropertyInitializerFunctionData);
}

std::string ParseCppTypeToSpellString(const cppast::cpp_type& CppType, bool bIsRemoveCV)
{
	std::string PtrOrRefString, TypeNameString, CVQualifiedString;
	const cppast::cpp_type* CppTypePtr = &CppType;
	if (CppTypePtr->kind() == cppast::cpp_type_kind::pointer_t)
	{
		auto CppPointerType = static_cast<const cppast::cpp_pointer_type*>(CppTypePtr);
		CppTypePtr = &CppPointerType->pointee();
		PtrOrRefString += "*";
	}
	if (CppTypePtr->kind() == cppast::cpp_type_kind::reference_t)
	{
		auto CppReferenceType = static_cast<const cppast::cpp_reference_type*>(CppTypePtr);
		CppTypePtr = &CppReferenceType->referee();
		PtrOrRefString += "&";
	}
	if (CppTypePtr->kind() == cppast::cpp_type_kind::cv_qualified_t)
	{
		auto CppCVQualifiedType = static_cast<const cppast::cpp_cv_qualified_type*>(CppTypePtr);
		CppTypePtr = &CppCVQualifiedType->type();
		if (!bIsRemoveCV)
		{
			auto CVQualifier = CppCVQualifiedType->cv_qualifier();
			switch (CVQualifier)
			{
			case cppast::cpp_cv_const:
				CVQualifiedString = "const";
				break;
			case cppast::cpp_cv_volatile:
				CVQualifiedString = "volatile";
				break;
			case cppast::cpp_cv_const_volatile:
				CVQualifiedString = "const volatile";
				break;
			case cppast::cpp_cv_none:
			default:
				break;
			}
		}
	}
	if (CppTypePtr->kind() == cppast::cpp_type_kind::builtin_t)
	{
		auto& CppBuiltinType = static_cast<const cppast::cpp_builtin_type&>(*CppTypePtr);
		TypeNameString = cppast::to_string(CppBuiltinType.builtin_type_kind());
	}
	else if (CppTypePtr->kind() == cppast::cpp_type_kind::user_defined_t)
	{
		auto& CppUserDefinedType = static_cast<const cppast::cpp_user_defined_type&>(*CppTypePtr);
		TypeNameString = CppUserDefinedType.entity().name();
	}
	else if (CppTypePtr->kind() == cppast::cpp_type_kind::template_instantiation_t)
	{
		auto& CppTemplateInstantiationType = static_cast<const cppast::cpp_template_instantiation_type&>(*CppTypePtr);
		if (CppTemplateInstantiationType.primary_template().name() == "std::vector")
		{
			auto& CppVectorElementType = CppTemplateInstantiationType.arguments().value().begin()->type().value();
			TypeNameString = "std::vector<" + ParseCppTypeToSpellString(CppVectorElementType, false) + ">";
		}
	}
	return CVQualifiedString + (CVQualifiedString.empty() ? "" : " ") + TypeNameString + PtrOrRefString;
}

bool IsRefType(const cppast::cpp_type& CppType)
{
	if (CppType.kind() == cppast::cpp_type_kind::reference_t)
	{
		return true;
	}
	return false;
}

