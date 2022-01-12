#pragma once
#include <string>
#include <unordered_map>
#include "MetadataApi.h"

class CMetadata;

struct CMetadataManager
{
private:
    CMetadataManager() = default;
public:
    static CMetadataManager& Instance();
protected:
    friend class CMetadata;
    void RegisterMetadata(CMetadata* Metadata);
    uint64_t IdCounter{ 0 };
    std::vector<CMetadata*> Metadatas;
};


class METADATA_API CMetadata
{
    friend class CMetadataManager;
public:
    CMetadata(const std::string& InName)
        : Name(InName)
    {
        CMetadataManager::Instance().RegisterMetadata(this);
    }
    virtual ~CMetadata() = default;

    std::string GetMetadataValue(const std::string& InKey)
    {
        if (auto it = Metadata.find(InKey); it != Metadata.end())
            return it->second;
        return nullptr;
    }

    bool ContainsMetadataKey(const std::string& InKey)
    {
        return Metadata.contains(InKey);
    }

    const std::string& GetName() { return Name; }

protected:
    uint64_t Id{UINT64_MAX};
    std::string Name;
    std::unordered_map<std::string, std::string> Metadata;
};
