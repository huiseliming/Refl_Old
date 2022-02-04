
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
"static CProperty* CLS_{{ClassName}}__PROP_{{PropertyName}}__STATIC_INITIALIZER()\n"
"{\n"
"    static {{PropertyTypeClass}} Prop(\"{{{PropertyName}}}\");\n"
"    Prop.SetAddressOffset({{PropertyAddressOffset}});\n"
"    Prop.SetFlag({{PropertyFlags}});\n"
"{{#MetadataKVList}}"
"    Prop.AddMetadata(\"{{{Key}}}\", \"{{{Value}}}\");\n"
"{{/MetadataKVList}}"
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
"    static CClass* Cls = {{ClassName}}::CLASS_STATIC_INITIALIZER();\n"
"    return Cls;\n"
"}\n"
"\n"
"CClass* {{ClassName}}::CLASS_STATIC_INITIALIZER()\n"
"{\n"
"    static CClass Cls;\n"
"    Cls.SetName(\"{{ClassName}}\");\n"
"    Cls.SetSize(sizeof({{ClassName}}));\n"
"{{#MetadataKVList}}"
"    Cls.AddMetadata(\"{{{Key}}}\", \"{{{Value}}}\");\n"
"{{/MetadataKVList}}"
"{{#PropertyInitializerList}}"
"    Cls.AddProperty(CLS_{{ClassName}}__PROP_{{PropertyName}}__STATIC_INITIALIZER());\n"
"{{/PropertyInitializerList}}"
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
"    Enum.SetSize(sizeof({{EnumName}}));\n"
"{{#MetadataKVList}}"
"    Enum.AddMetadata(\"{{{Key}}}\", \"{{{Value}}}\");\n"
"{{/MetadataKVList}}"
"{{#EnumKVList}}"
"    Enum.AddName({{{Value}}}, \"{{{Name}}}\");\n"
"{{/EnumKVList}}"
"    return &Enum;\n"
"}\n"
"\n"
"{{/EnumStaticInitializerList}}"
;

}
