# Refl

Refl为使用CMake的C++工程提供反射,其实现方式参考了UE4.

Refl能在CMake工程构建时会自动生成反射头文件和源文件添加到编译文件中.

Refl的反射数据部分在main执行之前已经初始化,剩下的部分需要在main函数中调用CType::ProcessPostStaticInitializerEvent完成初始化.

Refl目前仅在Windows上测试和使用,依赖库使用vcpkg集成.

-已实现
对反射记录附加元数据.
类属性的反射访问支持C++内建数据和类.

-进行中
类函数的反射访问

-计划中
类基于动态反射的Cast实现.
类属性的反射支持数组(参考UE4风格的TArray -> std::vector).
类属性的反射支持字典(参考UE4风格的TMap -> std::[unordered_]map).


struct FStruct
{
    // property...
}

// class 
class CClass
{
    // property...
    // function...
}

// Refl Object
class RPlayer : public RObject
{
    // property...
    // function...
}



