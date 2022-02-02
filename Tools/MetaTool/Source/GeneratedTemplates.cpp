
namespace GeneratedTemplates
{

const char* HeaderTemplate = 

"#pragma once\n"
"\n"
"{{#IncludeFileList}}"
"#include \"{{.}}\"\n"
"{{/IncludeFileList}}"
"\n"
""

;


const char* PropertyInitializerFunctionTemplate =
"static CProperty* CLS_{{ClassName}}__PROP_{{PropertyName}}__STATIC_INITIALIZER()\n"
"{\n"
"    static {{PropertyTypeClass}} Prop(\"{{{PropertyName}}}\");\n"
"    Prop.SetAddressOffset({{PropertyAddressOffset}});\n"
"    Prop.SetFlag({{PropertyFlags}});\n"
"{{{CustomExpression}}}"
"    return &Prop;\n"
"}\n"
"\n"
;

const char* SourceTemplate = 

"{{#IncludeFileList}}"
"#include \"{{.}}\"\n"
"{{/IncludeFileList}}"
"\n"
"\n"
"{{#ClassStaticInitializerList}}"
"{{#PropertyInitializerFunctionList}}"
"{{{.}}}"
"{{/PropertyInitializerFunctionList}}"
"\n"
"CClass* {{ClassName}}::StaticClass()\n"
"{\n"
"    static CClass* Class = {{ClassName}}::CLASS_STATIC_INITIALIZER();\n"
"    return Class;\n"
"}\n"
"\n"
"CClass* {{ClassName}}::CLASS_STATIC_INITIALIZER()\n"
"{\n"
"    CClass* Class = ::StaticClass<{{ClassName}}>();\n"
"    Class->SetName(\"{{ClassName}}\");\n"
"    Class->SetSize(sizeof({{ClassName}}));\n"
"{{#PropertyInitializerList}}"
"    Class->AddProperty(CLS_{{ClassName}}__PROP_{{PropertyName}}__STATIC_INITIALIZER());\n"
"{{/PropertyInitializerList}}"
"    return Class;\n"
"}\n"
"\n"
"TAutoInitializer<{{ClassName}}> CLS_{{ClassName}}_AUTO_INITIALIZER;\n"
"\n"
"{{/ClassStaticInitializerList}}"
;

}
