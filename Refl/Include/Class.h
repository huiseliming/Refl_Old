#pragma once
#include "Type.h"

class CProperty;
class CFunction;

class REFL_API CClass : public CType
{
public:
    CClass(const std::string& InName = "", uint32_t InSize = 0)
        : CType(InName, InSize)
    {
        if (!InName.empty()) SetName(InName);
        //Manager().RegisterType(this);
    }

    void AddBase(CType* Base)
    {
        Bases_.push_back(Base);
    }

    void AddProperty(CProperty* Property)
    {
        Propertie_.push_back(Property);
    }

    std::vector<CType*>& GetBases() { return Bases_; }
    std::vector<CProperty*>& GetProperties() { return Propertie_; }

private:
    std::vector<CType*> Bases_;
    //std::vector<CConstructor*> Constructors;
    std::vector<CProperty*> Propertie_;
    std::vector<CFunction*> Functions_;

};

//template<typename T>
//class TCppClass : public CClass
//{
//public:
//    TCppClass(const std::string& InName = "")
//        : CClass(InName, sizeof(T))
//    {}
//
//};
































