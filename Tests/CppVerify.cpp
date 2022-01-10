#include <iostream>
#include <string>
#include <any>
#include <tuple>
#include "Function.h"

int add(int a, int b)
{
	return a + b;
}


int main()
{
	int s = 10;
	int** ss = new int* ();
	*ss = &s;
	delete ss;

	return 0;
	TCppFunction<int, int, int> Function("add", &add);
	auto Frame = Function.NewFrame();
	int* a = (int*)Frame->GetArg(0);
	int* b = (int*)Frame->GetArg(1);
	*a = 3;
	*b = 4;
	Function.Invoke(Frame.get());
	int* c = (int*)Frame->GetRet();
	std::cout << *c << std::endl;
}

int print2(bool w)
{
	if (w)
	{
		printf("1");
	}
	else
	{
		printf("0");
	}
	return 1;
}

template<typename... Args>
void Print(Args&& ... args)
{
	std::tuple{ print2(std::forward<Args>(args))... };
}

int main1() 
{
	int www = 12;
	const int* a = &www;
	int const*  b = &www;
	int const*& rb = b;
	//*b = 1;
	int* const c = &www;
	//c = nullptr;
	int* d = &www;
	d = nullptr;
	std::cout << typeid(a).name() << std::endl;
	std::cout << typeid(b).name() << std::endl;
	std::cout << typeid(rb).name() << std::endl;
	std::cout << typeid(c).name() << std::endl;
	std::cout << typeid(d).name() << std::endl;
	int k = 2;
	int& rk = k;
	int* pk = &k;
	int** ppk = &pk;
	std::any any = ppk;
	std::cout << any.type().name() << std::endl;
	Print(true, false, false, false, true);
	std::cout << typeid(decltype(rb)).name() << std::endl;

	std::tuple<int&, int> TTTT(k, k);

	std::get<0>(TTTT) = 3;
	std::get<1>(TTTT) = 4;
	return 1;
}