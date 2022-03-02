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
        Bases_.push_back((CClass*)Base);
    }

    void AddProperty(CProperty* Property)
    {
        Properties_.push_back(Property);
    }

    void AddFunction(CFunction* Function)
    {
        Functions_.push_back(Function);
    }

    std::vector<CClass*>& GetBases() { return Bases_; }
    std::vector<CProperty*>& GetProperties() { return Properties_; }
    std::vector<CFunction*>& GetFunctions() { return Functions_; }

    CProperty* FindProperty(const std::string& PropertyName);

    CFunction* FindFunction(const std::string& FunctionName);

private:
    std::vector<CClass*> Bases_;
    //std::vector<CConstructor*> Constructors;
    std::vector<CProperty*> Properties_;
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
































