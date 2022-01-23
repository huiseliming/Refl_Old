#pragma once
#include "Type.h"

class METADATA_API CClass : public CType
{
public:
    CClass(const std::string& InName = "", uint32_t InSize = 0)
        : CType(InName, InSize)
    {
        //Manager().RegisterType(this);
    }

    void AddField(CField* InField)
    {
        Fields.push_back(InField);
    }

private:
    std::vector<CCppType*> Bases;
    //std::vector<CConstructor*> Constructors;
    std::vector<CField*> Fields;
    std::vector<CFunction*> Functions;
};

template<typename T>
class TCppClass : public CClass
{
public:
    TCppClass(const std::string& InName = "")
        : CClass(InName, sizeof(T))
    {}

};
































