#pragma once
#include <stack>
#include "Metadata.h"
#include "mustache.hpp"
#include "Helper.h"

class CCodeGenerator
{
protected:
public:
	CCodeGenerator();
	~CCodeGenerator();

	void GetHeaderIncludeFilesData();

	kainjow::mustache::data& ClassBegin(const std::string& InClassName)
	{
		ClassStaticInitializer_.set("ClassName", InClassName);
		return ClassStaticInitializer_;
	}

	void ClassEnd()
	{
		ClassStaticInitializer_.set("PropertyInitializerFunctionList", PropertyInitializerFunctionList_);
		ClassStaticInitializer_.set("FunctionInitializerList", FunctionInitializerList_);
		ClassStaticInitializer_.set("PropertyInitializerList", PropertyInitializerList_);
		ClassStaticInitializerList_.push_back(ClassStaticInitializer_);
		ClassStaticInitializer_ = kainjow::mustache::data();
	}

	kainjow::mustache::data& PropertyBegin(const std::string& InPropertyName)
	{
		PropertyInitializer_.set("PropertyName", InPropertyName);
		return PropertyInitializer_;
	}

	void PropertyEnd()
	{
		PropertyInitializerList_.push_back(PropertyInitializer_);
		PropertyInitializer_ = kainjow::mustache::data();
	}

	kainjow::mustache::data& FunctionBegin(const std::string& InFunctionName)
	{
		FunctionInitializer_.set("FunctionName", InFunctionName);
		return FunctionInitializer_;
	}

	void FunctionEnd()
	{
		FunctionInitializerList_.push_back(FunctionInitializer_);
		FunctionInitializer_ = kainjow::mustache::data();
	}

	std::string GenerateGeneratedFile()
	{
		SourceData_.set("ClassStaticInitializerList", ClassStaticInitializerList_);
		SourceData_.set("IncludeFileList", IncludeFileList_);
		return SourceTmpl_.render(SourceData_);
	}

	kainjow::mustache::mustache HeaderTmpl_;
	kainjow::mustache::mustache SourceTmpl_;

	kainjow::mustache::data ClassStaticInitializer_;
	kainjow::mustache::data ClassStaticInitializerList_{ kainjow::mustache::data::type::list };

	kainjow::mustache::data PropertyInitializer_;
	kainjow::mustache::data PropertyInitializerList_{ kainjow::mustache::data::type::list };
	kainjow::mustache::data PropertyInitializerFunctionList_{ kainjow::mustache::data::type::list };

	kainjow::mustache::data IncludeFileList_{ kainjow::mustache::data::type::list };

	kainjow::mustache::data FunctionInitializer_;
	kainjow::mustache::data FunctionInitializerList_{ kainjow::mustache::data::type::list };

	kainjow::mustache::data HeaderData_;
	kainjow::mustache::data SourceData_;
	std::vector<std::string> PropertyInitializerFunctions_;
	std::stack<std::function<void()>> ContainerEntityExitCallbackStack_;
private:
	std::stack<kainjow::mustache::data> MustacheDataStack;

};

