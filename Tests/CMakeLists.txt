

#add_executable(CppVerify CppVerify.cpp)

#target_link_libraries(CppVerify Metadata)

file(GLOB_RECURSE PublicHeader Include/*.h)
file(GLOB_RECURSE PrivateHeader Source/*.h)
file(GLOB_RECURSE PrivateSource Source/*.cpp)
list(FILTER PublicHeader  EXCLUDE REGEX "^.*\\.generated\\.h")
list(FILTER PrivateHeader EXCLUDE REGEX "^.*\\.generated\\.h")
list(FILTER PrivateSource EXCLUDE REGEX "^.*\\.generated\\.cpp")
list(FILTER PublicHeader  EXCLUDE REGEX "^.*Export\\.h")
foreach(HeaderFile ${PublicHeader} ${PrivateHeader})
    string(FIND ${HeaderFile} ".h" HeaderFileDotHPos REVERSE)
    string(SUBSTRING ${HeaderFile} 0 ${HeaderFileDotHPos} HeaderFileName)
    set(GeneratedHeaderFileFullName ${HeaderFileName})
    set(GeneratedSourceFileFullName ${HeaderFileName})
    string(APPEND GeneratedHeaderFileFullName ".generated.h")
    string(APPEND GeneratedSourceFileFullName ".generated.cpp")
    list(APPEND GeneratedHeader ${GeneratedHeaderFileFullName})
    list(APPEND GeneratedSource ${GeneratedSourceFileFullName})
    #message(${HeaderFile})
    #message( ${GeneratedHeaderFileFullName})
    #message( ${GeneratedSourceFileFullName})
    if(NOT EXISTS ${GeneratedHeaderFileFullName})
        file(WRITE ${GeneratedHeaderFileFullName}  "#pragma once")
    endif()
endforeach()

add_executable(
    Tests 
    ${PublicHeader}
    ${PrivateHeader}
    ${PrivateSource}
    ${GeneratedHeader}
    ${GeneratedSource}
    )

target_link_libraries(Tests PUBLIC Refl)
# for lib
#target_include_directories(Tests PUBLIC
#    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include>
#    $<INSTALL_INTERFACE:Include>    #${CMAKE_INSTALL_PREFIX}/include
#    )
#
#include(GenerateExportHeader)
#generate_export_header(Tests
#    EXPORT_MACRO_NAME TESTS_API
#    EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/Include/TestsExport.h)
#
#message(" --database_dir ${CMAKE_BINARY_DIR} --std c++20 --export_name Tests --macro_definition __METADATA__ ${PublicHeader} ${PrivateHeader}")


message(" --database_dir ${CMAKE_BINARY_DIR} --std c++20 --macro_definition __METADATA__ ${PublicHeader} ${PrivateHeader}")

foreach(InputHeader ${PublicHeader} ${PrivateHeader})
    list(APPEND InputHeaders ${InputHeader})
endforeach()

add_custom_target(check_tests_generated_file
    ALL
    COMMAND ReflGenerator 
    --database_dir ${CMAKE_BINARY_DIR} 
    --std c++20 
#    --export_name Tests 
    --macro_definition __METADATA__ 
    ${InputHeaders}
    DEPENDS ${InputHeaders}
    BYPRODUCTS ${GeneratedHeader} ${GeneratedSource}
    COMMENT "Checking the generated file for changes..."
    )
    
add_dependencies(check_tests_generated_file ReflGenerator)
add_dependencies(Tests check_tests_generated_file)

# add the command that invokes the reflection parser executablec
# whenever a header file in your project has changed
#add_custom_command(
#    OUTPUT ${META_GENERATED_HEADER} ${META_GENERATED_SOURCE}
#    DEPENDS ${PublicHeader} ${PrivateHeader}
#    COMMAND MetaTool
#    --target-name "${PROJECT_NAME}"
#    --in-source "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_SOURCE_DIR}/${PROJECT_META_HEADER}"
#    --out-header "${META_GENERATED_HEADER}"
#    --out-source "${META_GENERATED_SOURCE}"
#    --flags ${META_FLAGS}
#)
#
#add_custom_command(
#    OUTPUT  generated.cpp          # Treated as relative to CMAKE_CURRENT_BINARY_DIR
#    COMMAND mytool
#              ${CMAKE_CURRENT_SOURCE_DIR}/someInputFile.cpp.in
#              ${CMAKE_CURRENT_BINARY_DIR}/generated.cpp
#    DEPENDS someInputFile.cpp.in   # Treated as relative to CMAKE_CURRENT_SOURCE_DIR
#)