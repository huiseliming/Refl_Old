

find_package(LLVM CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

set(CLANG_INCLUDE_DIRS "llvm/clang/include")
set(CLANG_LIBS clang)

file(GLOB_RECURSE MetaToolSource Source/*.cpp Source/*.h)

add_executable(MetaTool ${MetaToolSource})

target_link_libraries(MetaTool PUBLIC Metadata)

#libclang configure BEGIN 
target_include_directories(MetaTool PUBLIC ${LLVM_INCLUDE_DIRS})
target_link_directories(MetaTool PUBLIC ${LLVM_LIBRARY_DIRS})
target_link_libraries(MetaTool PUBLIC libclang)
target_link_libraries(MetaTool PUBLIC fmt::fmt)
#target_link_libraries(MetaTool PUBLIC libclang ${CLANG_LIBS})

add_custom_command(TARGET MetaTool POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different     # which executes "cmake - E copy_if_different..."
    "${LLVM_BINARY_DIR}/bin/libclang.dll"             # <--this is in-file
    "${LLVM_BINARY_DIR}/bin/zlib1.dll"                # <--this is in-file
    $<TARGET_FILE_DIR:MetaTool>)                     # <--this is out-file path
#libclang configure BEGIN 

## export symbols
##include(GenerateExportHeader)
##generate_export_header(MetaDataGenerator EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/Include/MetaToolExport.h)
### export symbols include directories
##target_include_directories(MetaDataGenerator PUBLIC
##    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/Include> #for export header
##    )
#
#target_compile_definitions(MetaTool PUBLIC COMPILE_REFLECTOR META_GENERATOR)
#
#
#add_custom_command(TARGET MetaTool POST_BUILD        # Adds a post-build event to MyTest
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different             # which executes "cmake - E copy_if_different..."
#    "${CMAKE_CURRENT_SOURCE_DIR}/Example/MetaTest.h"          # <--this is in-file
#    "${CMAKE_CURRENT_SOURCE_DIR}/Example/MetaTest.cpp"        # <--this is in-file
#    $<TARGET_FILE_DIR:MetaTool>)                     # <--this is out-file path
#
##get_cmake_property(Vars VARIABLES)
##foreach(Var ${Vars})
##    message(STATUS "${Var}=${${Var}}")
##endforeach()
#