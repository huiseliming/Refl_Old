#include "CodeGenerator.h"

CCodeGenerator& CCodeGenerator::Instance()
{
	static CCodeGenerator CodeGenerator;
	return CodeGenerator;
}

