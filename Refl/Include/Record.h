#pragma once
#include <string>
#include <cassert>
#include <unordered_map>
#include "ReflExport.h"

#ifdef __REFL_GENERATED__
#define RMETADATA(...)  __attribute__((annotate("Metadata" __VA_OPT__(",") #__VA_ARGS__)))
#define RCLASS(...)     RMETADATA(__VA_ARGS__)
#define RSTRUCT(...)    RMETADATA(__VA_ARGS__)
#define RENUM(...)      RMETADATA(__VA_ARGS__)
#define RPROPERTY(...)  RMETADATA(__VA_ARGS__)
#define RFUNCTION(...)  RMETADATA(__VA_ARGS__)
#else
#define RCLASS(...)
#define RSTRUCT(...)
#define RENUM(...)
#define RPROPERTY(...)
#define RFUNCTION(...)
#define RMETADATA(...)
#endif

#define GENERATED_BODY()                   \
private:                                   \
static CClass* CLASS_STATIC_INITIALIZER(); \
public:                                    \
static CClass* StaticClass();              \
//static int64_t MetadataId;                 \

//
//struct CMetadataValue
//{
//    enum EType
//    {
//        EMVT_Integer,
//        EMVT_Float,
//        EMVT_String
//    };
//    CMetadataValue(double Value);
//
//    EType Type_;
//    std::string Value_;
//};


class REFL_API CRecord
{
    friend class CMetadataManager;
public:
    CRecord(const std::string& Name)
        : Name_(Name)
    {
        Records.push_back(this);
        Id_ = IdCounter++;
    }
    virtual ~CRecord() = default;

    std::string GetMetadataValue(const std::string& Key)
    {
        if (auto it = Metadatas_.find(Key); it != Metadatas_.end())
            return it->second;
        return nullptr;
    }

    bool ContainsMetadataKey(const std::string& Key)
    {
        return Metadatas_.contains(Key);
    }

    void AddMetadata(const std::string& Key, const std::string& Value)
    {
        if (Metadatas_.contains(Key)) assert(false);
        Metadatas_.insert_or_assign(Key, Value);
    }

    void SetName(const std::string& Name) { Name_ = Name; }

    const std::string& GetName() const { return Name_; }

    int64_t GetId() { return Id_; }

protected:
    int64_t Id_{INT64_MAX};
    std::string Name_;
    std::unordered_map<std::string, std::string> Metadatas_;

private:
    static int64_t IdCounter;
    static std::vector<CRecord*> Records;
};

template<typename From, typename To>
union TForceCast {
    From F;
    To T;
};

template<typename From, typename To>
To ForceCast(From F)
{
    TForceCast<From, To> U;
    U.F = F;
    return U.T;
}