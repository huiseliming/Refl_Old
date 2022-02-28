#include "Record.h"

int64_t CRecord::IdCounter = 0;
std::vector<CRecord*> CRecord::Records;

CRecord::CRecord(const std::string& Name)
    : Name_(Name)
{
    Records.push_back(this);
    Id_ = IdCounter++;
}

std::string CRecord::GetMetadataValue(const std::string& Key)
{
    if (auto it = Metadatas_.find(Key); it != Metadatas_.end())
        return it->second;
    return nullptr;
}

bool CRecord::ContainsMetadataKey(const std::string& Key)
{
    return Metadatas_.contains(Key);
}

void CRecord::AddMetadata(const std::string& Key, const std::string& Value)
{
    if (Metadatas_.contains(Key)) assert(false);
    Metadatas_.insert_or_assign(Key, Value);
}

CClass* GetRecordAsClass(int64_t Id)
{
    assert(CRecord::Records.size() > Id);
    return reinterpret_cast<CClass*>(CRecord::Records[Id]);
}