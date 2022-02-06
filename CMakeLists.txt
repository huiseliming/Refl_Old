cmake_minimum_required(VERSION 3.20)

if(NOT PROJECT_NAME)
    # export EvnVar VCPKG_ROOT 
    # set vcpkg triplet and toolchain_file, Must before project()
    if(WIN32)
        if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
            set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
        endif()
        if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
        set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
        elseif(NOT DEFINED VCPKG_TARGET_TRIPLET)
        set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "")
        endif()
        message("CMAKE_TOOLCHAIN_FILE: " ${CMAKE_TOOLCHAIN_FILE})
        message("VCPKG_TARGET_TRIPLET: " ${VCPKG_TARGET_TRIPLET})
    endif()

    project(Refl LANGUAGES CXX C)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY        ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY        ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY        ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)

if(WIN32)
    # set windows export symbols(spawn .lib)
    add_compile_options(/MP)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS_SAVE ${CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS})
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS OFF)
endif()

option(BuildMetadataCppTests "build Refl tests" ON)

add_subdirectory(ThirdParty)
add_subdirectory(Refl)
add_subdirectory(ReflGenerator)
if(BuildMetadataCppTests)
    add_subdirectory(Tests)
endif()

## add the command that invokes the reflection parser executable
## whenever a header file in your project has changed
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
