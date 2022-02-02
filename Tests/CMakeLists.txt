

#add_executable(CppVerify CppVerify.cpp)

#target_link_libraries(CppVerify Metadata)

file(GLOB_RECURSE PublicHeader Include/*.h)
file(GLOB_RECURSE PrivateHeader Source/*.h)
file(GLOB_RECURSE PrivateSource Source/*.cpp)

add_executable(
    Tests 
    ${PublicHeader}
    ${PrivateHeader}
    ${PrivateSource}
    )
target_link_libraries(Tests PUBLIC Metadata)


# add the command that invokes the reflection parser executable
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