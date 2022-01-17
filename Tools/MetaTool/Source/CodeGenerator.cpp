#include "CodeGenerator.h"
#include "GeneratedTemplates.h"

CCodeGenerator::CCodeGenerator()
	: HeaderTmpl(GeneratedTemplates::HeaderTemplate)
	, SourceTmpl(GeneratedTemplates::SourceTemplate)
{
}

CCodeGenerator::~CCodeGenerator()
{
	for (auto Medadata :Medadatas)
		delete Medadata;
}

CCodeGenerator& CCodeGenerator::Instance()
{
	static CCodeGenerator CodeGenerator;
	return CodeGenerator;
}

