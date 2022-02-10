#include "Object.h"


CObject* NewObject(CClass* Class)
{
	CObject* O = (CObject*)Class->New();
	O->SetClass(Class);
	return O;
}

