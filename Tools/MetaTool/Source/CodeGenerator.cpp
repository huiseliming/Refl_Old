#include "CodeGenerator.h"

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

