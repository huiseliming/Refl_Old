#pragma once 
#include "Type.h"
#include <map>

class CEnum : public CType
{
public:
    CEnum(const std::string& InName, uint32_t InSize = 0)
        : CType(InName, InSize)
    {
        //Manager().RegisterType(this);
    }

    void AddEnumValue(uint64_t InValue, std::string InName);
    const std::string& ToName(uint64_t InValue);
    uint64_t ToValue(const std::string& InName);

private:
    std::unordered_map<uint64_t, std::string> ValueToName_;
    std::unordered_map<std::string, uint64_t> NameToValue_;
    
};
