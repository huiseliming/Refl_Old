
file(GLOB_RECURSE PublicHeader Include/*.h)
file(GLOB_RECURSE PrivateHeader Source/*.h)
file(GLOB_RECURSE PrivateSource Source/*.cpp)

add_library(Refl SHARED ${PublicHeader} ${PrivateHeader} ${PrivateSource})

target_include_directories(Refl PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include>
    $<INSTALL_INTERFACE:Include>    #${CMAKE_INSTALL_PREFIX}/include
)

# For windows export symbols
include(GenerateExportHeader)
generate_export_header(Refl 
    EXPORT_MACRO_NAME REFL_API
    EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/Include/ReflExport.h)
if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ${CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS_SAVE})
endif()





