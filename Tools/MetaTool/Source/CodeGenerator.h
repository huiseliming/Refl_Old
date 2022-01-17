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

	template<typename T>
	T* RequiredMetadata(const std::string& MetadataName = "")
	{
		T* Metadata = new T(MetadataName);
		Medadatas.push_back(new T(MetadataName));
		return Metadata;
	}

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
	

	kainjow::mustache::mustache HeaderTmpl;
	kainjow::mustache::mustache SourceTmpl;
	kainjow::mustache::data HeaderIncludeFile{ kainjow::mustache::data::type::list };
	kainjow::mustache::data SourceIncludeFile{ kainjow::mustache::data::type::list };
	kainjow::mustache::data ClassStaticInitializer{ kainjow::mustache::data::type::list };

private:
	std::stack<CMetadata*> MetadataStack;
	uint64_t IdCounter{ 0 };
	std::vector<CMetadata*> Medadatas;
};

