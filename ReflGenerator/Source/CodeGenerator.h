#pragma once
#include <stack>
#include "Refl.h"
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
		ClassStaticInitializer_.set("ExpressionList", ClassStaticInitializerExpressionList_);
		ClassStaticInitializerExpressionList_= kainjow::mustache::data{ kainjow::mustache::data::type::list };
		ClassStaticInitializerList_.push_back(ClassStaticInitializer_);
		ClassStaticInitializer_ = kainjow::mustache::data();
	}

	kainjow::mustache::data& EnumBegin(const std::string& EnumName)
	{
		EnumStaticInitializer_.set("EnumName", EnumName);
		return EnumStaticInitializer_;
	}

	void EnumEnd()
	{
		EnumStaticInitializer_.set("ExpressionList", EnumStaticInitializerExpressionList_);
		EnumStaticInitializerExpressionList_ = kainjow::mustache::data{ kainjow::mustache::data::type::list };
		EnumStaticInitializerList_.push_back(EnumStaticInitializer_);
		EnumStaticInitializer_ = kainjow::mustache::data();
	}

	kainjow::mustache::data& PropertyBegin(const std::string& InPropertyName)
	{
		PropertyInitializer_.set("PropertyName", InPropertyName);
		return PropertyInitializer_;
	}

	void PropertyEnd()
	{
		PropertyInitializer_.set("ExpressionList", PropertyInitializerExpressionList_);
		PropertyInitializerExpressionList_ = kainjow::mustache::data{ kainjow::mustache::data::type::list };
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
		FunctionInitializer_.set("ExpressionList", FunctionInitializerExpressionList_);
		FunctionInitializerExpressionList_ = kainjow::mustache::data{ kainjow::mustache::data::type::list };
		FunctionInitializerList_.push_back(FunctionInitializer_);
		FunctionInitializer_ = kainjow::mustache::data();
	}

	std::string GenerateGeneratedFile()
	{
		SourceData_.set("GlobalExpressionList", GlobalExpressionList_);
		SourceData_.set("ClassStaticInitializerList", ClassStaticInitializerList_);
		SourceData_.set("IncludeFileList", IncludeFileList_);
		SourceData_.set("EnumStaticInitializerList", EnumStaticInitializerList_);
		return SourceTmpl_.render(SourceData_);
	}

	kainjow::mustache::mustache HeaderTmpl_;
	kainjow::mustache::mustache SourceTmpl_;

	kainjow::mustache::data GlobalExpressionList_{ kainjow::mustache::data::type::list };
	
	kainjow::mustache::data ClassStaticInitializerExpressionList_{ kainjow::mustache::data::type::list };
	kainjow::mustache::data ClassStaticInitializer_;
	kainjow::mustache::data ClassStaticInitializerList_{ kainjow::mustache::data::type::list };

	kainjow::mustache::data PropertyInitializerExpressionList_{ kainjow::mustache::data::type::list };
	kainjow::mustache::data PropertyInitializer_;
	kainjow::mustache::data PropertyInitializerList_{ kainjow::mustache::data::type::list };

	kainjow::mustache::data FunctionInitializerExpressionList_{ kainjow::mustache::data::type::list };
	kainjow::mustache::data FunctionInitializer_;
	kainjow::mustache::data FunctionInitializerList_{ kainjow::mustache::data::type::list };

	kainjow::mustache::data EnumKVList_{ kainjow::mustache::data::type::list };
	kainjow::mustache::data EnumDisplayNameList_{ kainjow::mustache::data::type::list };

	kainjow::mustache::data EnumStaticInitializerExpressionList_{ kainjow::mustache::data::type::list };
	kainjow::mustache::data EnumStaticInitializer_;
	kainjow::mustache::data EnumStaticInitializerList_{ kainjow::mustache::data::type::list };


	kainjow::mustache::data IncludeFileList_{ kainjow::mustache::data::type::list };


	kainjow::mustache::data HeaderData_;
	kainjow::mustache::data SourceData_;
	std::vector<std::string> PropertyInitializerFunctions_;
	std::stack<std::function<void()>> ContainerEntityExitCallbackStack_;
private:
	std::stack<kainjow::mustache::data> MustacheDataStack;

};

