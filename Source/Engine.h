#pragma once

#include "Metadata.h"



class CLASS("dsd", 1) CTestMetadataClass
{
public:
	PROPERTY()
	bool TestBoolProperty;
	PROPERTY()
	int32_t TestSInt32Property;
	PROPERTY()
	int64_t TestSInt64Property;
	PROPERTY()
	int32_t TestUInt32Property;
	PROPERTY()
	int64_t TestUInt64Property;
	PROPERTY()
	float TestFloatProperty;
	PROPERTY()
	double TestDoubleProperty;
	PROPERTY()
	std::string TestStringProperty;
};

class CLASS() CTimerManager
{
public:

};

class CLASS() CEngine
{

	


	CTimerManager TimerManager;
};

