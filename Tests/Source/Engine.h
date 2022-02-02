#pragma once

#include "MetadataApi.h"
#include "Engine.generated.h" 

class CLASS() CTimerManager
{
	GENERATED_BODY()
public:

};

class CLASS() CEngine
{
	GENERATED_BODY()
public:
	CTimerManager TimerManager;
};

//class CEngine;

class CLASS("dsd", 1) CTestMetadataClass
{
	GENERATED_BODY()
public:
	//PROPERTY()
	//bool TestBoolProperty;
	//PROPERTY()
	//int32_t TestSInt32Property;
	//PROPERTY()
	//int64_t TestSInt64Property;
	PROPERTY()
	const volatile int32_t& TestUInt32Property;

	PROPERTY()
	const volatile CEngine* TestEnginePtrProperty;
	//PROPERTY()
	//int64_t TestUInt64Property;
	//PROPERTY()
	//float TestFloatProperty;
	//PROPERTY()
	//double TestDoubleProperty;
	//PROPERTY()
	//std::string TestStringProperty;

	//PROPERTY()
	//const volatile int32_t* TestCVPtrProperty;
	//PROPERTY()
	//int32_t* TestPtrProperty;
	//PROPERTY()
	//std::vector<const volatile int32_t*> TestCVPtrVectorProperty;
	PROPERTY()
	std::vector<int32_t*> TestPtrVectorProperty;

	//PROPERTY()
	//std::vector<CEngine*> TestEnginePtrVectorProperty;

	//PROPERTY()
	//const volatile int32_t& TestCVPtrProperty;
	//PROPERTY()
	//int32_t& TestPtrProperty;
};
