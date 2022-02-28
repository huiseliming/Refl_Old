#include "Enum.h"
#include "StaticType.h"

static std::vector<std::string> EmptyVector;

void CEnum::AddName(uint64_t InValue, const std::string& InName)
{
    Values_.insert(InValue);
    ValueToName_[InValue].push_back(InName);
    assert(!NameToValue_.contains(InName));
    NameToValue_.insert(std::make_pair(InName, InValue));
}

void CEnum::AddDisplayName(uint64_t InValue, const std::string& InDisplayName)
{
    assert(!NameToValue_.contains(InDisplayName));
    NameToValue_.insert(std::make_pair(InDisplayName, InValue));
    ValueToDisplayName_.insert(std::make_pair(InValue, InDisplayName));
}

const std::vector<std::string>& CEnum::ToName(uint64_t InValue)
{
    auto It = ValueToName_.find(InValue);
    if (It != ValueToName_.end())
    {
        return It->second;
    }
    return EmptyVector;
}

const std::string& CEnum::ToDisplayName(uint64_t InValue)
{
    auto It = ValueToDisplayName_.find(InValue);
    if (It != ValueToDisplayName_.end())
        return It->second;
    auto& Names = ToName(InValue);
    if (!Names.empty())
        return Names.front();
    return CStaticString::Empty;
}

uint64_t CEnum::ToValue(const std::string& InName)
{
    auto It = NameToValue_.find(InName);
    if (It != NameToValue_.end())
    {
        return It->second;
    }
    return UINT64_MAX;
}

std::set<uint64_t>& CEnum::GetEnumValues()
{
    return Values_;
}
