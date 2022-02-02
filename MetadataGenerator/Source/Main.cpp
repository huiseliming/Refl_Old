#include <thread>
#include <filesystem>
#include <fstream>
#include <cxxopts.hpp>

#include <cppast/code_generator.hpp>         // for generate_code()
#include <cppast/cpp_entity_kind.hpp>        // for the cpp_entity_kind definition
#include <cppast/cpp_forward_declarable.hpp> // for is_definition()
#include <cppast/cpp_member_variable.hpp>
#include <cppast/cpp_namespace.hpp>          // for cpp_namespace
#include <cppast/libclang_parser.hpp> // for libclang_parser, libclang_compile_config, cpp_entity,...
#include <cppast/visitor.hpp>         // for visit()
#include <cppast/cpp_member_function.hpp>
#include <cppast/cpp_template.hpp>
#include <fmt/format.h>

#include "StaticType.h"
#include "CodeGenerator.h"
#include "Helper.h"
#include "Type.h"
#include "Class.h"
#include "Property.h"
#include "Parser.h"
#include "GeneratedTemplates.h"

// prints the AST entry of a cpp_entity (base class for all entities),
// will only print a single line
void PrintEntity(std::ostream& out, const cppast::cpp_entity& e)
{
    if (true)
    {

    }


    // print name and the kind of the entity
    if (!e.name().empty())
        out << e.name();
    else
        out << "<anonymous>";
    out << " (" << cppast::to_string(e.kind()) << ")";

    // print whether or not it is a definition
    if (cppast::is_definition(e))
        out << " [definition]";

    // print number of attributes
    if (!e.attributes().empty())
        out << " [" << e.attributes().size() << " attribute(s)]";

    if (e.kind() == cppast::cpp_entity_kind::language_linkage_t)
        // no need to print additional information for language linkages
        out << '\n';
    else if (e.kind() == cppast::cpp_entity_kind::namespace_t)
    {
        // cast to cpp_namespace
        auto& ns = static_cast<const cppast::cpp_namespace&>(e);
        // print whether or not it is inline
        if (ns.is_inline())
            out << " [inline]";
        out << '\n';
    }
    else
    {
        // print the declaration of the entity
        // it will only use a single line
        // derive from code_generator and implement various callbacks for printing
        // it will print into a std::string
        class code_generator : public cppast::code_generator
        {
            std::string str_;                 // the result
            bool        was_newline_ = false; // whether or not the last token was a newline
            // needed for lazily printing them

        public:
            code_generator(const cppast::cpp_entity& e)
            {
                // kickoff code generation here
                cppast::generate_code(*this, e);
            }

            // return the result
            const std::string& str() const noexcept
            {
                return str_;
            }

        private:
            // called to retrieve the generation options of an entity
            generation_options do_get_options(const cppast::cpp_entity&,
                cppast::cpp_access_specifier_kind) override
            {
                // generate declaration only
                return code_generator::declaration;
            }

            // no need to handle indentation, as only a single line is used
            void do_indent() override {}
            void do_unindent() override {}

            // called when a generic token sequence should be generated
            // there are specialized callbacks for various token kinds,
            // to e.g. implement syntax highlighting
            void do_write_token_seq(cppast::string_view tokens) override
            {
                if (was_newline_)
                {
                    // lazily append newline as space
                    str_ += ' ';
                    was_newline_ = false;
                }
                // append tokens
                str_ += tokens.c_str();
            }

            // called when a newline should be generated
            // we're lazy as it will always generate a trailing newline,
            // we don't want
            void do_write_newline() override
            {
                was_newline_ = true;
            }

        } generator(e);
        // print generated code
        out << ": `" << generator.str() << '`' << '\n';
    }
}


// prints the AST of a file
void PrintAst(const cppast::cpp_entity_index& idx, std::ostream& out, const cppast::cpp_file& file)
{
    // print file name
    out << "AST for '" << file.name() << "':\n";
    std::string prefix; // the current prefix string
    // recursively visit file and all children
    cppast::visit(file, [&](const cppast::cpp_entity& e, cppast::visitor_info info) {
        if (e.kind() == cppast::cpp_entity_kind::file_t || cppast::is_templated(e) || cppast::is_friended(e))
        {
            // no need to do anything for a file,
            // templated and friended entities are just proxies, so skip those as well
            // return true to continue visit for children
            return true;
        }
        else if (info.event == cppast::visitor_info::container_entity_exit)
        {
            // we have visited all children of a container,
            // remove prefix
            prefix.pop_back();
            prefix.pop_back();
            if(CCodeGenerator::Instance().ContainerEntityExitCallbackStack_.top())
                CCodeGenerator::Instance().ContainerEntityExitCallbackStack_.top()();
            CCodeGenerator::Instance().ContainerEntityExitCallbackStack_.pop();
        }
        else
        {
            out << prefix; // print prefix for previous entities
            // calculate next prefix
            if (info.last_child)
            {
                if (info.event == cppast::visitor_info::container_entity_enter)
                    prefix += "  ";
                out << "+-";
            }
            else
            {
                if (info.event == cppast::visitor_info::container_entity_enter)
                    prefix += "| ";
                out << "|-";
            }
            PrintEntity(out, e);

            std::function<void()> ContainerEntityExitCallback;
            std::function<void()> Hanle;
            bool bIsRequiredMetadata = false;
            if (!e.attributes().empty())
            {
                for (const cppast::cpp_attribute& attribute : e.attributes())
                {
                    if (attribute.name() == "annotate")
                    {
                        if (attribute.arguments().has_value())
                        {
                            auto AttributeArguments = attribute.arguments().value();
                            if (!AttributeArguments.empty() && AttributeArguments.front().spelling == "\"Metadata\"")
                            {
                                bIsRequiredMetadata = true;
                            }
                        }
                    }
                }
            }
            if (bIsRequiredMetadata)
            {
                CCodeGenerator& CodeGenerator = CCodeGenerator::Instance();
                if (e.kind() == cppast::cpp_entity_kind::class_t)
                {
                    auto& CppClass = static_cast<const cppast::cpp_class&>(e);
                    CCodeGenerator::Instance().ClassBegin(CppClass.name());
                    ContainerEntityExitCallback = []{ CCodeGenerator::Instance().ClassEnd(); };
                    //CType* Type = CodeGenerator.RequiredMetadata<CType>(e.name());
                    //CodeGenerator.PushMetadata(StaticClass<CType>());
                }
                else if (e.kind() == cppast::cpp_entity_kind::member_variable_t)
                {
                    std::string ClassName = CCodeGenerator::Instance().ClassStaticInitializer_.get("ClassName")->string_value();
                    CPropertyInfo PropertyInfo = {};
                    auto& CppMemberVariable = static_cast<const cppast::cpp_member_variable&>(e);
                    auto& PropertyData = CCodeGenerator::Instance().PropertyBegin(CppMemberVariable.name());
                    auto& CppMemberVariableType = CppMemberVariable.type();
                    const cppast::cpp_type* TypePtr = &CppMemberVariableType;
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
                                CPropertyInfo VectorElementPropertyInfo = ParseCppTypeToPropertyInfo(idx, CppMemberVariableVectorType);
                                kainjow::mustache::mustache VectorElementPropertyInitializerFunctionTmpl(GeneratedTemplates::PropertyInitializerFunctionTemplate);
                                kainjow::mustache::data VectorElementPropertyInitializerFunctionData;
                                VectorElementPropertyInitializerFunctionData.set("ClassName", ClassName);
                                VectorElementPropertyInitializerFunctionData.set("PropertyName", CppMemberVariable.name() + "__VectorElement");
                                VectorElementPropertyInitializerFunctionData.set("PropertyTypeClass", ToPropertyTypeName(VectorElementPropertyInfo.PropertyFlag));
                                VectorElementPropertyInitializerFunctionData.set("PropertyAddressOffset", "0");
                                VectorElementPropertyInitializerFunctionData.set("PropertyFlags", fmt::format("{:#x}", VectorElementPropertyInfo.PropertyFlag));
                                if (VectorElementPropertyInfo.PropertyFlag & EPF_ClassFlag)
                                {
                                    VectorElementPropertyInitializerFunctionData.set("CustomExpression",
                                        "    CType::PostStaticInitializerList().push_back([&]{\n"
                                        "        assert(CType::NameToType.contains(\"" + VectorElementPropertyInfo.PropertyClassName + "\"));\n"
                                        "        Prop.SetClass(reinterpret_cast<CClass*>(CType::NameToType[\"" + VectorElementPropertyInfo.PropertyClassName + "\"]));\n"
                                        "    });\n"
                                    );
                                }
                                CCodeGenerator::Instance().PropertyInitializerFunctionList_.push_back(VectorElementPropertyInitializerFunctionTmpl.render(VectorElementPropertyInitializerFunctionData));
                                PropertyInfo.PropertyFlag |= EPF_VectorFlag;
                            }
                        }
                        //Field->QualifiedType.Type = ToString(CppMemberVariableBuiltinType.builtin_type_kind());
                        //cppast::to_string(cppast::cpp_type_kind::pointer_t);
                    }
                    else
                    {
                        PropertyInfo = ParseCppTypeToPropertyInfo(idx, CppMemberVariableType);
                    }
                    if (!(PropertyInfo.PropertyFlag & EPF_TypeFlagBits))
                    {
                        PropertyInfo.PropertyFlag |= EPF_UnknowFlag;
                    }
                    kainjow::mustache::mustache PropertyInitializerFunctionTmpl(GeneratedTemplates::PropertyInitializerFunctionTemplate);
                    kainjow::mustache::data PropertyInitializerFunctionData;
                    PropertyInitializerFunctionData.set("ClassName", ClassName);
                    PropertyInitializerFunctionData.set("PropertyName", CppMemberVariable.name());
                    PropertyInitializerFunctionData.set("PropertyTypeClass", ToPropertyTypeName(PropertyInfo.PropertyFlag));
                    PropertyInitializerFunctionData.set("PropertyAddressOffset", fmt::format("offsetof({}, {}::{})", ClassName, ClassName, CppMemberVariable.name()));
                    PropertyInitializerFunctionData.set("PropertyFlags", fmt::format("{:#x}", PropertyInfo.PropertyFlag));
                    if (PropertyInfo.PropertyFlag & EPF_ClassFlag)
                    {
                        PropertyInitializerFunctionData.set("CustomExpression",
                            "    CType::PostStaticInitializerList().push_back([&]{\n"
                            "        assert(CType::NameToType.contains(\"" + PropertyInfo.PropertyClassName + "\"));\n"
                            "        Prop.SetClass(reinterpret_cast<CClass*>(CType::NameToType[\"" + PropertyInfo.PropertyClassName + "\"]));\n"
                            "    });\n"
                        );
                    }
                    else if (PropertyInfo.PropertyFlag & EPF_VectorFlag)
                    {
                        PropertyInitializerFunctionData.set("CustomExpression",
                            "    Prop.SetDataProperty(CLS_" + ClassName + "__PROP_" + CppMemberVariable.name() + "__VectorElement__STATIC_INITIALIZER());\n"
                        );
                    }
                    CCodeGenerator::Instance().PropertyInitializerFunctionList_.push_back(PropertyInitializerFunctionTmpl.render(PropertyInitializerFunctionData));
                    CCodeGenerator::Instance().PropertyInitializer_.set("ClassName", ClassName);
                    CCodeGenerator::Instance().PropertyInitializer_.set("PropertyName", CppMemberVariable.name());
                    CCodeGenerator::Instance().PropertyEnd();
                }
                else if (e.kind() == cppast::cpp_entity_kind::member_function_t)
                {
                    auto& CppMemberFunction = static_cast<const cppast::cpp_member_function&>(e);
                    CCodeGenerator::Instance().FunctionBegin(CppMemberFunction.name());
                    ContainerEntityExitCallback = [] { CCodeGenerator::Instance().FunctionEnd(); };
                }
            }
            if (info.event == cppast::visitor_info::container_entity_enter)
            {
                CCodeGenerator::Instance().ContainerEntityExitCallbackStack_.push(ContainerEntityExitCallback);
            }
        }

        return true;
        });
}


// parse a file
type_safe::optional_ref<const cppast::cpp_file> ParseFile(const cppast::cpp_entity_index& idx, const cppast::libclang_compile_config& config,
    const cppast::diagnostic_logger& logger,
    const std::string& filename, bool fatal_error)
{
    // the entity index is used to resolve cross references in the AST
    // we don't need that, so it will not be needed afterwards
    //cppast::cpp_entity_index idx;
    // the parser is used to parse the entity
    // there can be multiple parser implementations

    cppast::simple_file_parser<cppast::libclang_parser> parser(type_safe::ref(idx), type_safe::ref(logger));
    //cppast::libclang_parser parser(type_safe::ref(logger));
    // parse the file
    auto file = parser.parse(filename, config);
    if (fatal_error && parser.error())
        return nullptr;
    cppast::resolve_includes(parser, file.value(), config);
    return file;
}

template <class FileParser>
std::size_t resolve_includes2(FileParser& parser, const cppast::cpp_file& file,
    typename FileParser::config config)
{
    auto count = 0u;
    for (auto& entity : file)
    {
        if (entity.kind() == cppast::cpp_include_directive::kind())
        {
            auto& include = static_cast<const cppast::cpp_include_directive&>(entity);
            auto file_opt = parser.parse(include.full_path(), config);
            resolve_includes(parser, file_opt.value(), config);
            ++count;
        }
    }
    return count;
}

int main(int argc, char** argv) try
{
    int ArgC;
    char** ArgV;
    std::string currentPath = std::filesystem::current_path().string();
    if (argc == 1) {
        static std::vector<const char*> CommandLines = {
            "",
            "--database_dir", "../",
            "--std", "c++20",
            "--macro_definition", "__METADATA__",
            "../../Tests/Source/Engine.h",
            "../../Tests/Source/Engine.cpp",
        };
        CommandLines[0] = argv[0];
        ArgC = CommandLines.size();
        ArgV = (char**)CommandLines.data();
    }
    else
    {
        ArgC = argc;
        ArgV = argv;
    }
    CCodeGenerator& CodeGenerator = CCodeGenerator::Instance();
    cxxopts::Options option_list("MetaTool",
        "MetaTool for CppEngine!\n");
    // clang-format off
    option_list.add_options()
        ("h,help", "display this help and exit")
        ("version", "display version information and exit")
        ("v,verbose", "be verbose when parsing")
        ("fatal_errors", "abort program when a parser error occurs, instead of doing error correction")
        ("file", "the file that is being parsed (last positional argument)",
            cxxopts::value<std::string>());
    option_list.add_options("compilation")
        ("database_dir", "set the directory where a 'compile_commands.json' file is located containing build information",
            cxxopts::value<std::string>())
        ("database_file", "set the file name whose configuration will be used regardless of the current file name",
            cxxopts::value<std::string>())
        ("std", "set the C++ standard (c++98, c++03, c++11, c++14, c++1z (experimental), c++17, c++2a, c++20)",
            cxxopts::value<std::string>()->default_value(cppast::to_string(cppast::cpp_standard::cpp_latest)))
        ("I,include_directory", "add directory to include search path",
            cxxopts::value<std::vector<std::string>>())
        ("D,macro_definition", "define a macro on the command line",
            cxxopts::value<std::vector<std::string>>())
        ("U,macro_undefinition", "undefine a macro on the command line",
            cxxopts::value<std::vector<std::string>>())
        ("f,feature", "enable a custom feature (-fXX flag)",
            cxxopts::value<std::vector<std::string>>())
        ("gnu_extensions", "enable GNU extensions (equivalent to -std=gnu++XX)")
        ("msvc_extensions", "enable MSVC extensions (equivalent to -fms-extensions)")
        ("msvc_compatibility", "enable MSVC compatibility (equivalent to -fms-compatibility)")
        ("fast_preprocessing", "enable fast preprocessing, be careful, this breaks if you e.g. redefine macros in the same file!")
        ("remove_comments_in_macro", "whether or not comments generated by macro are kept, enable if you run into errors");
    // clang-format on
    option_list.parse_positional("file");
    auto options = option_list.parse(ArgC, ArgV);
    if (options.count("help"))
        std::cout << option_list.help({ "", "compilation" }) << '\n';
    else if (options.count("version"))
    {
        std::cout << "MetaTools version ?.?.?\n";
        std::cout << "Using cppast version " << CPPAST_VERSION_STRING << '\n';
        std::cout << "Using libclang version " << CPPAST_CLANG_VERSION_STRING << '\n';
    }
    else if (!options.count("file") || options["file"].as<std::string>().empty())
    {
        std::cerr << "missing file argument\n";
        return 1;
    }
    else
    {
        // the compile config stores compilation flags
        cppast::libclang_compile_config config;
        if (options.count("database_dir"))
        {
            cppast::libclang_compilation_database database(
                options["database_dir"].as<std::string>());
            if (options.count("database_file"))
                config
                = cppast::libclang_compile_config(database,
                    options["database_file"].as<std::string>());
            else
                config
                = cppast::libclang_compile_config(database, options["file"].as<std::string>());
        }

        if (options.count("verbose"))
            config.write_preprocessed(true);

        if (options.count("fast_preprocessing"))
            config.fast_preprocessing(true);

        if (options.count("remove_comments_in_macro"))
            config.remove_comments_in_macro(true);

        if (options.count("include_directory"))
            for (auto& include : options["include_directory"].as<std::vector<std::string>>())
                config.add_include_dir(include);
        if (options.count("macro_definition"))
            for (auto& macro : options["macro_definition"].as<std::vector<std::string>>())
            {
                auto equal = macro.find('=');
                auto name = macro.substr(0, equal);
                if (equal == std::string::npos)
                    config.define_macro(std::move(name), "");
                else
                {
                    auto def = macro.substr(equal + 1u);
                    config.define_macro(std::move(name), std::move(def));
                }
            }
        if (options.count("macro_undefinition"))
            for (auto& name : options["macro_undefinition"].as<std::vector<std::string>>())
                config.undefine_macro(name);
        if (options.count("feature"))
            for (auto& name : options["feature"].as<std::vector<std::string>>())
                config.enable_feature(name);

        // the compile_flags are generic flags
        cppast::compile_flags flags;
        if (options.count("gnu_extensions"))
            flags |= cppast::compile_flag::gnu_extensions;
        if (options.count("msvc_extensions"))
            flags |= cppast::compile_flag::ms_extensions;
        if (options.count("msvc_compatibility"))
            flags |= cppast::compile_flag::ms_compatibility;

        if (options["std"].as<std::string>() == "c++98")
            config.set_flags(cppast::cpp_standard::cpp_98, flags);
        else if (options["std"].as<std::string>() == "c++03")
            config.set_flags(cppast::cpp_standard::cpp_03, flags);
        else if (options["std"].as<std::string>() == "c++11")
            config.set_flags(cppast::cpp_standard::cpp_11, flags);
        else if (options["std"].as<std::string>() == "c++14")
            config.set_flags(cppast::cpp_standard::cpp_14, flags);
        else if (options["std"].as<std::string>() == "c++1z")
            config.set_flags(cppast::cpp_standard::cpp_1z, flags);
        else if (options["std"].as<std::string>() == "c++17")
            config.set_flags(cppast::cpp_standard::cpp_17, flags);
        else if (options["std"].as<std::string>() == "c++2a")
            config.set_flags(cppast::cpp_standard::cpp_2a, flags);
        else if (options["std"].as<std::string>() == "c++20")
            config.set_flags(cppast::cpp_standard::cpp_20, flags);
        else
        {
            std::cerr << "invalid value '" << options["std"].as<std::string>() << "' for std flag\n";
            return 1;
        }

        // the logger is used to print diagnostics
        cppast::stderr_diagnostic_logger logger;
        if (options.count("verbose"))
            logger.set_verbose(true);
        cppast::cpp_entity_index idx;
        cppast::simple_file_parser<cppast::libclang_parser> parser(type_safe::ref(idx), type_safe::ref(logger));
        //cppast::libclang_parser parser(type_safe::ref(logger));
        // parse the file
        auto file = parser.parse(options["file"].as<std::string>(), config);
        if (options.count("fatal_errors") == 1 && parser.error())
            return 3;
        resolve_includes2(parser, file.value(), config);
        //auto file = ParseFile(idx, config, logger, options["file"].as<std::string>(),
        //    options.count("fatal_errors") == 1);
        //if (!file)
        //    return 2;
        PrintAst(idx, std::cout, file.value());


        std::string InputFileFullPath = std::filesystem::weakly_canonical(options["file"].as<std::string>()).string();
        // header 
        std::string OuputHeaderFileFullPath = GetOuputHeaderFileFullPath(InputFileFullPath);
        std::fstream OuputHeaderFileStream;
        OuputHeaderFileStream.open(OuputHeaderFileFullPath, std::ios::out | std::ios::trunc);
        if (!OuputHeaderFileStream.is_open())
        {
            throw std::runtime_error(OuputHeaderFileFullPath + "cant open for  std::ios::out | std::ios::trunc");
        }

        kainjow::mustache::mustache HeaderTmpl(GeneratedTemplates::HeaderTemplate);
        kainjow::mustache::data HeaderTmplData;
        kainjow::mustache::data IncludeFileList(kainjow::mustache::data::type::list);
        IncludeFileList.push_back(InputFileFullPath);
        HeaderTmplData.set("IncludeFileList", IncludeFileList);
        std::string GeneratedHeader = HeaderTmpl.render(HeaderTmplData);
        OuputHeaderFileStream.write(GeneratedHeader.data(), GeneratedHeader.size());

        //source
        std::string OuputSourceFileFullPath = GetOuputSourceFileFullPath(InputFileFullPath);
        std::fstream OuputSourceFileStream;
        OuputSourceFileStream.open(OuputSourceFileFullPath, std::ios::out | std::ios::trunc);
        if (!OuputSourceFileStream.is_open())
        {
            throw std::runtime_error(OuputSourceFileFullPath + "cant open for  std::ios::out | std::ios::trunc");
        }
        CCodeGenerator::Instance().IncludeFileList_.push_back(OuputHeaderFileFullPath);
        std::string GeneratedSource = CCodeGenerator::Instance().GenerateGeneratedFile();
        OuputSourceFileStream.write(GeneratedSource.data(), GeneratedSource.size());

        //kainjow::mustache::data HeaderTmplData;
        //CodeGenerator.HeaderTmpl.render(CodeGenerator.HeaderIncludeFile);
        //std::string HeaderOutput = CodeGenerator.HeaderTmpl.render(HeaderTmplData);
        //std::cout << HeaderOutput << std::endl;
        //kainjow::mustache::data SourceTmplData;
        //SourceTmplData.list_value().
        //SourceTmplData.set("IncludeFiles", CodeGenerator.SourceIncludeFile);
        //SourceTmplData.set("ClassStaticInitializer", CodeGenerator.ClassStaticInitializer);
        //std::string SourceOutput = CodeGenerator.SourceTmpl.render(SourceTmplData);
        //std::cout << SourceOutput << std::endl;
    }
}
catch (const cppast::libclang_error& ex)
{
    std::cout << "[fatal parsing error] "<< ex.what() << "\n";
    return 2;
}

