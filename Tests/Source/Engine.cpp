#include "Engine.h"
#include <iostream>

int main()
{
    CType::PostStaticInitializer();
    CClass* Class = CTestMetadataClass::StaticClass();
    for (size_t i = 0; i < Class->GetProperties().size(); i++)
    {
        CProperty* Property = Class->GetProperties()[i];
        std::cout << Property->GetName() << std::hex << Property->GetFlag() << std::dec <<  std::endl;
    }
}