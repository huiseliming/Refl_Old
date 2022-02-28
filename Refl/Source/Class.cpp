#include "Class.h"
#include "Function.h"

CProperty* CClass::FindProperty(const std::string& PropertyName)
{
    for (size_t i = 0; i < Properties_.size(); i++)
    {
        if (Properties_[i]->GetName() == PropertyName)
        {
            return Properties_[i];
        }
    }
    return nullptr;
}


CFunction* CClass::FindFunction(const std::string& FunctionName)
{
    for (size_t i = 0; i < Functions_.size(); i++)
    {
        if (Functions_[i]->GetName() == FunctionName)
        {
            return Functions_[i];
        }
    }
    return nullptr;
}


