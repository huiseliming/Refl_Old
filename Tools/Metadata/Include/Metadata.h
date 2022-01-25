#pragma once
#include <string>
#include <cassert>
#include <unordered_map>
#include "MetadataApi.h"

#ifdef __METADATA__
#define METADATA(...)  __attribute__((annotate("Metadata" __VA_OPT__(",") #__VA_ARGS__)))
#define CLASS(...)     METADATA(__VA_ARGS__)
#define STRUCT(...)    METADATA(__VA_ARGS__)
#define ENUM(...)      METADATA(__VA_ARGS__)
#define PROPERTY(...)  METADATA(__VA_ARGS__)
#define FUNCTION(...)  METADATA(__VA_ARGS__)
#else
#define CLASS(...)
#define STRUCT(...)
#define ENUM(...)
#define PROPERTY(...)
#define FUNCTION(...)
#define METADATA(...)
#endif

#define GENERATED_BODY()                   \
public:                                    \
static CClass* CLASS_STATIC_INITIALIZER(); \
static CClass* StaticClass();              \
static int64_t MetadataId;                 \

class METADATA_API CMetadata
{
    friend class CMetadataManager;
public:
    CMetadata(const std::string& InName)
        : Name(InName)
    {
        Metadatas.push_back(this);
        Id = IdCounter++;
    }
    virtual ~CMetadata() = default;

    std::string GetMetadataValue(const std::string& InKey)
    {
        if (auto it = KeyToValue.find(InKey); it != KeyToValue.end())
            return it->second;
        return nullptr;
    }

    bool ContainsMetadataKey(const std::string& InKey)
    {
        return KeyToValue.contains(InKey);
    }

    void AddData(const std::string& InKey, const std::string& InValue)
    {
        if (KeyToValue.contains(InKey)) assert(false);
        KeyToValue.insert_or_assign(InKey, InValue);
    }

    void SetName(const std::string& InName) { Name = InName; }

    const std::string& GetName() { return Name; }

protected:
    int64_t Id{INT64_MAX};
    std::string Name;
    std::unordered_map<std::string, std::string> KeyToValue;

private:
    static int64_t IdCounter;
    static std::vector<CMetadata*> Metadatas;
};

