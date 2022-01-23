#pragma once
#include <stack>
#include "Metadata.h"
#include "mustache.hpp"
#include "Helper.h"

class CCodeGenerator
{
protected:
	CCodeGenerator();
	~CCodeGenerator();
public:
	static CCodeGenerator& Instance();

	void PushMetadata(CMetadata* Metadata)
	{
		MetadataStack.push(Metadata);
	}

	template<typename T = CMetadata>
	T* GetTopMetadata()
	{
		return reinterpret_cast<T*>(MetadataStack.top());
	}

	void PopMetadata()
	{
		MetadataStack.pop();
	}
	

	void PushMustacheData(CMetadata* Metadata)
	{
		MustacheDataStack.push(Metadata);
	}

	template<typename T = CMetadata>
	T* GetTopMustacheData()
	{
		return reinterpret_cast<T*>(MustacheDataStack.top());
	}

	void PopMustacheData()
	{
		MustacheDataStack.pop();
	}

	void GetHeaderIncludeFilesData();

	void ClassBegin(const std::string& InClassName)
	{
		ClassStaticInitializer.set("ClassName", InClassName);
	}

	void ClassEnd()
	{
		ClassStaticInitializer.set("FunctionInitializerList", FunctionInitializerList);
		ClassStaticInitializer.set("PropertyInitializerList", PropertyInitializerList);
		ClassStaticInitializerList.push_back(ClassStaticInitializer);
		ClassStaticInitializer = kainjow::mustache::data();
	}

	void PropertyBegin(const std::string& InPropertyName)
	{
		PropertyInitializer.set("PropertyName", InPropertyName);
	}

	void PropertyEnd()
	{
		PropertyInitializerList.push_back(PropertyInitializer);
		PropertyInitializer = kainjow::mustache::data();
	}

	void FunctionBegin(const std::string& InFunctionName)
	{
		FunctionInitializer.set("FunctionName", InFunctionName);
	}

	void FunctionEnd()
	{
		FunctionInitializerList.push_back(FunctionInitializer);
		FunctionInitializer = kainjow::mustache::data();
	}

	std::string GenerateGeneratedFile()
	{
		SourceData.set("ClassStaticInitializerList", ClassStaticInitializerList);
		return SourceTmpl.render(SourceData);
	}

	kainjow::mustache::mustache HeaderTmpl;
	kainjow::mustache::mustache SourceTmpl;

	kainjow::mustache::data ClassStaticInitializer;
	kainjow::mustache::data ClassStaticInitializerList{ kainjow::mustache::data::type::list };

	kainjow::mustache::data PropertyInitializer;
	kainjow::mustache::data PropertyInitializerList{ kainjow::mustache::data::type::list };

	kainjow::mustache::data FunctionInitializer;
	kainjow::mustache::data FunctionInitializerList{ kainjow::mustache::data::type::list };

	kainjow::mustache::data HeaderData;
	kainjow::mustache::data SourceData;

	std::stack<std::function<void()>> ContainerEntityExitCallbackStack;
private:

	std::stack<CMetadata*> MetadataStack;
	std::stack<kainjow::mustache::data> MustacheDataStack;

};

