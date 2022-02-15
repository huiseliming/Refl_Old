#include "Class.h"
#include "Function.h"

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


