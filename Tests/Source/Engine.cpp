#include "Engine.h"
#include <iostream>

int main()
{
    CType::ProcessPostStaticInitializerEvent();
    CClass* Class = CTestMetadataClass::StaticClass();
    for (size_t i = 0; i < Class->GetProperties().size(); i++)
    {
        CProperty* Property = Class->GetProperties()[i];
        std::cout << Property->GetName() << std::hex << Property->GetFlag() << std::dec <<  std::endl;
    }
    auto Enum = TEnum<ETestEnum>::StaticEnum();
    auto& Values = Enum->GetValues();
    for (auto & Value : Values)
    {
        std::cout << Value << " : ";
        auto& Names = Enum->ToName(Value);
        for (size_t i = 0; i < Names.size(); i++)
        {
            std::cout  << Names[i] << ", ";
        }
        std::cout << std::endl;
    }
}