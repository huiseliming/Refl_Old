#include "Type.h"
#include "cxxopts.hpp"
#include <thread>
#include <filesystem>
#include "CodeGenerator.h"
#include <clang-c/Index.h>
#include <clang-c/CXCompilationDatabase.h>

int main(int ArgC, const char* ArgV[])
{
    std::string CompileCommandsDataBaseDir;
    std::vector<std::string> InputFiles;
    std::string OutputDir;
    cxxopts::Options Options("MetaTool", "MetaTool for CppEngine!");
    Options.add_options()("c,compile_commands", "compile_commands.json file dir", cxxopts::value(CompileCommandsDataBaseDir));
    Options.add_options()("f,source_file", "input source_file", cxxopts::value(InputFiles));
    Options.add_options()("o,output_dir", "output dir", cxxopts::value(OutputDir));

    if (CompileCommandsDataBaseDir.empty())
    {
        std::cout << "please set compile_commands" << std::endl;
        return 0;
    }
    if (InputFiles.empty())
    {
        std::cout << "please set file" << std::endl;
        return 0;
    }
    if (OutputDir.empty())
    {
        std::cout << "please set output " << std::endl;
        return 0;
    }
    if (!std::filesystem::exists(OutputDir)) {
        std::filesystem::create_directories(OutputDir);
    }
    CXCompilationDatabase_Error CompilationDatabase_Error;
    CXCompilationDatabase CompilationDatabase = clang_CompilationDatabase_fromDirectory(CompileCommandsDataBaseDir.c_str(), &CompilationDatabase_Error);
    if (CompilationDatabase_Error == CXCompilationDatabase_CanNotLoadDatabase)
    {
        std::cerr << "clang_CompilationDatabase_fromDirectory(" << CompileCommandsDataBaseDir << ", " << (CompilationDatabase_Error == CXCompilationDatabase_NoError ?  "NoError" : "CanNotLoadDatabase") << ")>\n";
        return -1;
    }
    std::vector<std::thread> TUVisitorThreads;

}



