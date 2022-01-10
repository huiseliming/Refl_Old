#pragma once
#include <string>
#include <unordered_map>
#include "MetadataApi.h"

class METADATA_API CMetadata
{
    static std::unordered_map<uint64_t, CMetadata*> Table;
public:
    CMetadata(const std::string& name)
        : Name(name)
    {}

    std::string GetMetadataValue(const std::string& key)
    {
        if (auto it = Metadata.find(key); it != Metadata.end())
            return it->second;
        return nullptr;
    }

    bool ContainsMetadataKey(const std::string& key)
    {
        return Metadata.contains(key);
    }

    const std::string& GetName() { return Name; }

protected:
    std::string Name;
    std::unordered_map<std::string, std::string> Metadata;
};
