#pragma once

#include "Refl.h"
#include "TimerManager.h"
#include "Engine.generated.h"


enum ENUM(ClassName = "测试枚举") ETestEnum
{
	Test1 METADATA(DisplayName = "测试1"),
	Test2 METADATA(DisplayName = "测试2"),
	Test3 METADATA(DisplayName = "测试3"),
	Test4 METADATA(DisplayName = "测试4"),
	Test5 METADATA(DisplayName = "测试522"),
};

#ifdef ENUM_ETestEnum_GENERATED_BODY
	ENUM_ETestEnum_GENERATED_BODY 
#endif // ENUM_ETestEnum_GENERATED_BODY

enum class ENUM() ETestEnum2 : uint64_t
{
	Test21,
	Test22,
	Test23,
	Test24,
	Test25,
};

#ifdef ENUM_ETestEnum2_GENERATED_BODY
	ENUM_ETestEnum2_GENERATED_BODY
#endif // ENUM_ETestEnum2_GENERATED_BODY

class CA{


};
 
class CLASS() CEngine
{
	GENERATED_BODY()
public:
	CTimerManager TimerManager;
};

//class CEngine;
 
class CLASS(ClassName = "测试类") CTestMetadataClass : public CEngine, CA
{
	GENERATED_BODY()
public:
	//PROPERTY()
	//bool TestBoolProperty;
	//PROPERTY()
	//int32_t TestSInt32Property;
	//PROPERTY()
	//int64_t TestSInt64Property;
	PROPERTY((ClassName = "测试属性"))
	const volatile int32_t& TestUInt32Property;

	PROPERTY(meta = "ds")
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
	PROPERTY((ClassName = "测试属性PtrVector"))
	std::vector<int32_t*> TestPtrVectorProperty;

	PROPERTY((ClassName = "测试属性EnginePtrVector"))
	std::vector<CEngine*> TestEnginePtrVectorProperty;

	//PROPERTY()
	//const volatile int32_t& TestCVPtrProperty;
	//PROPERTY()
	//int32_t& TestPtrProperty;
};
