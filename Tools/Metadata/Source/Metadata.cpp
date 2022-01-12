#include "Metadata.h"

void CMetadataManager::RegisterMetadata(CMetadata* Metadata)
{
	Metadatas.push_back(Metadata);
	Metadata->Id = IdCounter++;
}

CMetadataManager& CMetadataManager::Instance()
{
	static CMetadataManager Mgr;
	return Mgr;
}

