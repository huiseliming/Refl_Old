#include "CodeGenerator.h"
#include "GeneratedTemplates.h"

CCodeGenerator::CCodeGenerator()
	: HeaderTmpl(GeneratedTemplates::HeaderTemplate)
	, SourceTmpl(GeneratedTemplates::SourceTemplate)
{
}

CCodeGenerator::~CCodeGenerator()
{
}

CCodeGenerator& CCodeGenerator::Instance()
{
	static CCodeGenerator CodeGenerator;
	return CodeGenerator;
}

