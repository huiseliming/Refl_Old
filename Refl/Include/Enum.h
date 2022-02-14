#pragma once 
#include "Type.h"
#include <map>
#include <set>

class REFL_API CEnum : public CType
{
public:
    CEnum(const std::string& InName = "", uint32_t InSize = 0)
        : CType(InName, InSize)
    {
    }
    

    void AddName(uint64_t InValue, const std::string& InName);
    void AddDisplayName(uint64_t InValue, const std::string& InDisplayName);
    const std::vector<std::string>& ToName(uint64_t InValue);
    const std::string& ToDisplayName(uint64_t InValue);
    uint64_t ToValue(const std::string& InName);
    std::set<uint64_t>& GetValues();

private:
    std::set<uint64_t> Values_;
    std::unordered_map<uint64_t, std::vector<std::string>> ValueToName_;
    std::unordered_map<std::string, uint64_t> NameToValue_;
    std::unordered_map<uint64_t, std::string> ValueToDisplayName_;
    
};
