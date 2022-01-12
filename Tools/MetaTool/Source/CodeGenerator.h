#pragma once
#include <stack>
#include "Metadata.h"

class CCodeGenerator
{
protected:
	CCodeGenerator() = default;
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

private:
	std::stack<CMetadata*> MetadataStack;
	uint64_t IdCounter{ 0 };
	std::vector<CMetadata*> Medadatas;
};

