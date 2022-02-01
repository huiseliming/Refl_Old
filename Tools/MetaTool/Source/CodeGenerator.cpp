#include "CodeGenerator.h"
#include "GeneratedTemplates.h"

CCodeGenerator::CCodeGenerator()
	: HeaderTmpl_(GeneratedTemplates::HeaderTemplate)
	, SourceTmpl_(GeneratedTemplates::SourceTemplate)
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

