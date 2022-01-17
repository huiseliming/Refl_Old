#include "Enum.h"

void CEnum::AddEnumValue(uint64_t InValue, std::string InName)
{
    ValueToName.insert(std::make_pair(InValue, InName));
    NameToValue.insert(std::make_pair(InName, InValue));
}

const std::string& CEnum::ToName(uint64_t InValue)
{
    return ValueToName[InValue];
}

uint64_t CEnum::ToValue(const std::string& InName)
{
    return NameToValue[InName];
}
