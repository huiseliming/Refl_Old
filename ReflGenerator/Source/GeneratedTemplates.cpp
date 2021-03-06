
namespace GeneratedTemplates
{

const char* HeaderTemplate = 

"#pragma once\n"
"\n"
"{{#IncludeFileList}}"
"#include \"{{.}}\"\n"
"{{/IncludeFileList}}"
"\n"
"{{#EnumStaticInitializerList}}"
"#define ENUM_{{EnumName}}_GENERATED_BODY \\\n"
"template<>\\\n"
"struct {{ExportApi}} TEnum<{{EnumName}}>\\\n"
"{\\\n"
"    static CEnum* StaticEnum();\\\n"
"    static CEnum* ENUM_STATIC_INITIALIZER();\\\n"
"};\\\n"
"\n"
"{{/EnumStaticInitializerList}}"
"\n"
;


const char* PropertyInitializerFunctionTemplate =
"static CProperty* {{PropertyStaticInitializerFunctionName}}()\n"
"{\n"
"    static {{PropertyTypeClass}} Prop(\"{{{PropertyName}}}\");\n"
"    Prop.SetAddressOffset({{PropertyAddressOffset}});\n"
"    Prop.SetFlag({{PropertyFlags}});\n"
"{{#ExpressionList}}"
"{{{.}}}"
"{{/ExpressionList}}"
"    return &Prop;\n"
"}\n"
"\n"
;

const char* ReflFunctionCallTemplate =
"struct {{ClassName}}_{{FunctionName}}_FRAME_STRUCT {\n"
"{{#FrameStructMemberList}}"
"    {{{.}}};\n"
"{{/FrameStructMemberList}}"
"};\n"
"\n"
"static CFunction* CLS_{{ClassName}}__FUNC_{{FunctionName}}__STATIC_INITIALIZER();\n"
"\n"
"{{#ReflFunctionGlobalExpressionList}}"
"{{{.}}}"
"{{/ReflFunctionGlobalExpressionList}}"
"\n"

"static void CLS_{{ClassName}}__FUNC_{{FunctionName}}__REFL_INVOKE(void* InputObj, void* FrameStructPtr)\n"
"{\n"
"    {{ClassName}}* Obj = reinterpret_cast<{{ClassName}}*>(InputObj);\n"
"    {{ClassName}}_{{FunctionName}}_FRAME_STRUCT* FS = ({{ClassName}}_{{FunctionName}}_FRAME_STRUCT*)FrameStructPtr;\n"
"    {{ReturnValueEqual}}Obj->{{FunctionName}}({{{FrameStructInputs}}});\n"
"}\n"
"\n"
"static CFunction* CLS_{{ClassName}}__FUNC_{{FunctionName}}__STATIC_INITIALIZER()\n"
"{\n"
"    static std::function<CFunction*()> Initializer = [] () -> CFunction* {\n"
"        static CFunction Func(\"{{{FunctionName}}}\");\n"
"        Func.SetReflInvokePtr(&CLS_{{ClassName}}__FUNC_{{FunctionName}}__REFL_INVOKE);\n"
"        Func.SetRowInvokePtr(ForceCast<decltype(&{{ClassName}}::{{FunctionName}}), void*>(&{{ClassName}}::{{FunctionName}}));\n"
"        Func.SetFrameSize(sizeof({{ClassName}}_{{FunctionName}}_FRAME_STRUCT));\n"
"    {{#ExpressionList}}"
"        {{{.}}}"
"    {{/ExpressionList}}"
"        return &Func;\n"
"    };\n"
"    static CFunction* F = Initializer();\n"
"    return F;\n"
"}\n"
"\n"
;

const char* SourceTemplate = 

"{{#IncludeFileList}}"
"#include \"{{.}}\"\n"
"{{/IncludeFileList}}"
"\n"
"{{#GlobalExpressionList}}"
"{{{.}}}"
"{{/GlobalExpressionList}}"
"\n"
"{{#ClassStaticInitializerList}}"
"CClass* {{ClassName}}::StaticClass()\n"
"{\n"
"    static CClass* Cls = {{ClassName}}::CLASS_STATIC_INITIALIZER();\n"
"    return Cls;\n"
"}\n"
"\n"
"CClass* {{ClassName}}::CLASS_STATIC_INITIALIZER()\n"
"{\n"
"    static CClass Cls;\n"
"    Cls.SetName(\"{{ClassName}}\");\n"
"{{#ExpressionList}}"
"{{{.}}}"
"{{/ExpressionList}}"
"    return &Cls;\n"
"}\n"
"\n"
"TAutoInitializer<{{ClassName}}> CLS_{{ClassName}}_AUTO_INITIALIZER;\n"
"\n"
"{{/ClassStaticInitializerList}}"
"{{#EnumStaticInitializerList}}"
"CEnum* TEnum<{{EnumName}}>::StaticEnum()\n"
"{\n"
"    static CEnum* Enum = TEnum<{{EnumName}}>::ENUM_STATIC_INITIALIZER();\n"
"    return Enum;\n"
"}\n"
"\n"
"CEnum* TEnum<{{EnumName}}>::ENUM_STATIC_INITIALIZER()\n"
"{\n"
"    static CEnum Enum;\n"
"    Enum.SetName(\"{{EnumName}}\");\n"
"    Enum.SetName(\"{{EnumName}}\");\n"
"{{#ExpressionList}}"
"{{{.}}}"
"{{/ExpressionList}}"
"    return &Enum;\n"
"}\n"
"\n"
"TAutoInitializer<{{EnumName}}> ENUM_{{EnumName}}_AUTO_INITIALIZER;\n"
"\n"
"{{/EnumStaticInitializerList}}"
;

}
