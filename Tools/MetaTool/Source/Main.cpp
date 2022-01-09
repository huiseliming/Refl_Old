#include <vector>
#include <string>
#include <memory>
#include <unordered_map>



class CMetadata
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

class CField : public CMetadata
{
public:
    CField(const std::string& name)
        : CMetadata(name)
    {}

};

class CFunction : public CMetadata
{
public:
    CFunction(const std::string& name)
        : CMetadata(name)
    {}
};

class CConstructor : public CFunction
{
    CConstructor(const std::string& name)
        : CFunction(name)
    {}
};

class CType : public CMetadata
{
public:
    CType(const std::string& name)
        : CMetadata(name)
    {}

private:
    std::vector<CType*> BaseTypes;
    std::vector<CConstructor*> Constructors;
    std::vector<CField*> Fields;
    std::vector<CFunction*> Functions;
};



std::unordered_map<uint64_t, CMetadata*> CMetadata::Table;

int main()
{

}



