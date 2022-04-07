

find_package(LLVM CONFIG REQUIRED)
find_package(cxxopts CONFIG REQUIRED)

set(CLANG_INCLUDE_DIRS "llvm/clang/include")
set(CLANG_LIBS clang)

file(GLOB_RECURSE ReflGeneratorSource Source/*.cpp Source/*.h)


add_executable(ReflGenerator ${ReflGeneratorSource})

target_link_libraries(ReflGenerator PUBLIC Refl)

#libclang configure BEGIN 
target_include_directories(ReflGenerator PUBLIC ${LLVM_INCLUDE_DIRS})
target_link_directories(ReflGenerator PUBLIC ${LLVM_LIBRARY_DIRS})
target_link_libraries(ReflGenerator PRIVATE libclang)
target_link_libraries(ReflGenerator PRIVATE cxxopts::cxxopts)
target_link_libraries(ReflGenerator PRIVATE cppast)
#target_link_libraries(ReflGenerator PUBLIC libclang ${CLANG_LIBS})

#file(READ ${CMAKE_CURRENT_SOURCE_DIR}/Template/GeneratedHeader.template GENERATED_HEADER_TEMPLATE_STRING)
#file(READ ${CMAKE_CURRENT_SOURCE_DIR}/Template/GeneratedSource.template GENERATED_SOURCE_TEMPLATE_STRING)
#string(REPLACE "\"" "\\\"" GENERATED_HEADER_TEMPLATE_STRING ${GENERATED_HEADER_TEMPLATE_STRING})
#string(REPLACE "\n" "\\n\"\n\"" GENERATED_HEADER_TEMPLATE_STRING ${GENERATED_HEADER_TEMPLATE_STRING})
#string(REPLACE "\"" "\\\"" GENERATED_SOURCE_TEMPLATE_STRING ${GENERATED_SOURCE_TEMPLATE_STRING})
#string(REPLACE "\n" "\\n\"\n\"" GENERATED_SOURCE_TEMPLATE_STRING ${GENERATED_SOURCE_TEMPLATE_STRING})
#configure_file(Template/GeneratedTemplates.cpp.template ${CMAKE_CURRENT_BINARY_DIR}/GeneratedTemplates.cpp)
#
#add_library(GeneratedTemplates STATIC ${CMAKE_CURRENT_BINARY_DIR}/GeneratedTemplates.cpp)
#target_link_libraries(ReflGenerator PRIVATE GeneratedTemplates)

target_compile_definitions(ReflGenerator PUBLIC META_TOOL)

add_custom_command(TARGET ReflGenerator POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different     # which executes "cmake - E copy_if_different..."
    "${LLVM_BINARY_DIR}/bin/libclang.dll"             # <--this is in-file
    "${LLVM_BINARY_DIR}/bin/zlib1.dll"                # <--this is in-file
    $<TARGET_FILE_DIR:ReflGenerator>)                     # <--this is out-file path
#libclang configure BEGIN 

## export symbols
##include(GenerateExportHeader)
##generate_export_header(ReflGenerator EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/Include/ReflGeneratorExport.h)
### export symbols include directories
##target_include_directories(ReflGenerator PUBLIC
##    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/Include> #for export header
##    )
#
#target_compile_definitions(ReflGenerator PUBLIC COMPILE_REFLECTOR META_GENERATOR)
#
#
#add_custom_command(TARGET ReflGenerator POST_BUILD        # Adds a post-build event to MyTest
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different             # which executes "cmake - E copy_if_different..."
#    "${CMAKE_CURRENT_SOURCE_DIR}/Example/MetaTest.h"          # <--this is in-file
#    "${CMAKE_CURRENT_SOURCE_DIR}/Example/MetaTest.cpp"        # <--this is in-file
#    $<TARGET_FILE_DIR:ReflGenerator>)                     # <--this is out-file path
#
##get_cmake_property(Vars VARIABLES)
##foreach(Var ${Vars})
##    message(STATUS "${Var}=${${Var}}")
##endforeach()
#