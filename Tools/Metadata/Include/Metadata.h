#pragma once
#include <string>
#include <cassert>
#include <unordered_map>
#include "MetadataApi.h"

#ifdef __REFLECT__
#define CLASS(...)     __attribute__((annotate("Metadata" __VA_OPT__(,) #__VA_ARGS__)))
#define STRUCT(...)    __attribute__((annotate("Metadata" __VA_OPT__(,) #__VA_ARGS__)))
#define ENUM(...)      __attribute__((annotate("Metadata" __VA_OPT__(,) #__VA_ARGS__)))
#define PROPERTY(...)  __attribute__((annotate("Metadata" __VA_OPT__(,) #__VA_ARGS__)))
#define FUNCTION(...)  __attribute__((annotate("Metadata" __VA_OPT__(,) #__VA_ARGS__)))
#define METADATA(...)  __attribute__((annotate("Metadata" __VA_OPT__(,) #__VA_ARGS__)))
#else
#define CLASS(...)
#define STRUCT(...)
#define ENUM(...)
#define PROPERTY(...)
#define FUNCTION(...)
#define METADATA(...)
#endif

#define GENERATED_META_BODY()                        \
public:                                              \
static FClass* StaticClass();                        \
static FMeta* StaticMeta();                          \
static UInt32 MetaId;

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

    void AddMetadata(const std::string& InKey, const std::string& InValue)
    {
        if (Metadata.contains(InKey)) assert(false);
        Metadata.insert_or_assign(InKey, InValue);
    }
    
    void SetName(const std::string& InName) { Name = InName; }

    const std::string& GetName() { return Name; }

protected:
    uint64_t Id{UINT64_MAX};
    std::string Name;
    std::unordered_map<std::string, std::string> Metadata;
};
