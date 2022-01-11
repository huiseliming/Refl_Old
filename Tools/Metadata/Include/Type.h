#pragma once
#include "Metadata.h"
#include "MetaValue.h"
#include "Function.h"

class METADATA_API CType : public CMetadata
{
public:
    CType(const std::string& name)
        : CMetadata(name)
    {}

private:
    std::vector<CType*       > BaseTypes;
    //std::vector<CConstructor*> Constructors;
    std::vector<CField*      > Fields;
    std::vector<CFunction*   > Functions;
};
