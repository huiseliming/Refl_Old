

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