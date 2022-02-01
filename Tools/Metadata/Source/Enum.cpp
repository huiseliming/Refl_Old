#include "Enum.h"

void CEnum::AddEnumValue(uint64_t InValue, std::string InName)
{
    ValueToName_.insert(std::make_pair(InValue, InName));
    NameToValue_.insert(std::make_pair(InName, InValue));
}

const std::string& CEnum::ToName(uint64_t InValue)
{
    return ValueToName_[InValue];
}

uint64_t CEnum::ToValue(const std::string& InName)
{
    return NameToValue_[InName];
}
