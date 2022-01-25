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


    void AddProperty(CProperty* Property)
    {
        Properties.push_back(Property);
    }


private:
    std::vector<CType*> Bases;
    //std::vector<CConstructor*> Constructors;
    std::vector<CProperty*> Properties;
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
































