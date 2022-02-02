#pragma once
#include <string>
#include <cassert>
#include <unordered_map>
#include "MetadataExport.h"

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
    CMetadata(const std::string& Name)
        : Name_(Name)
    {
        Metadatas.push_back(this);
        Id_ = IdCounter++;
    }
    virtual ~CMetadata() = default;

    std::string GetMetadataValue(const std::string& Key)
    {
        if (auto it = KeyToValue_.find(Key); it != KeyToValue_.end())
            return it->second;
        return nullptr;
    }

    bool ContainsMetadataKey(const std::string& Key)
    {
        return KeyToValue_.contains(Key);
    }

    void AddData(const std::string& Key, const std::string& Value)
    {
        if (KeyToValue_.contains(Key)) assert(false);
        KeyToValue_.insert_or_assign(Key, Value);
    }

    void SetName(const std::string& Name) { Name_ = Name; }

    const std::string& GetName() const { return Name_; }

protected:
    int64_t Id_{INT64_MAX};
    std::string Name_;
    std::unordered_map<std::string, std::string> KeyToValue_;

private:
    static int64_t IdCounter;
    static std::vector<CMetadata*> Metadatas;
};

