#include <iostream>
#include <string>
#include <any>
#include <tuple>
#include "Function.h"


int add(int a, int& b)
{
	b = 8;
	return a + b;
}

int simple(int& a)
{
	return a;
}

void* ArgsPtr[10];

//template<typename T>
//T To(void* A)
//{
//	return *(T*)A
//};

//template<typename ... Args>
//class CC
//{
//public:
//	template <size_t... indices>
//	static std::tuple<Args...> MakeArgsTuple(std::index_sequence<indices...> indexSequence)
//	{
//		return std::make_tuple((*()ArgsPtr[indices])...);
//	}
//
//};

template<typename T, typename ... Args>
struct C
{
	using Tuple = std::tuple<std::conditional_t<std::is_reference_v<Args>, std::reference_wrapper<std::remove_reference_t<Args>>, Args> ...>;

};


int main()
{
	//std::cout << typeid(std::conditional_t<std::is_reference_v<int&>, std::reference_wrapper<std::remove_reference_t<int&>>, int&>).name() << std::endl;
	//std::cout << typeid(std::conditional_t<std::is_reference_v<int>, std::reference_wrapper<std::remove_reference_t<int>>, int>).name() << std::endl;

	//int sddsd  = 1;
	//int sddsd2 = 3;
	//std::tuple<int, int&> ssss(1, sddsd);
	//std::get<1>(ssss) = sddsd2;
	//std::get<1>(ssss) = 6;

	//int fdfd = std::apply(&add, std::tuple<int, std::reference_wrapper<int>>(sddsd, sddsd2));

	//ssss = std::make_tuple(2, sddsd);
	//std::string j4 = "dsadas";
	//int j1 = 1, j2 = 2, j3 = 3;
	//std::tuple<int&&, int, int&, std::string&&> scc(1, j2, j3, "1525564");
	//std::get<0>(scc) = 4;
	//std::get<1>(scc) = 5;
	//std::get<2>(scc) = 6;
	//std::get<3>(scc) = "dsdsdsdsdsds";
	//std::cout << std::is_reference_v       <int*&> << std::endl;
	//std::cout << std::is_rvalue_reference_v<int*&> << std::endl;
	//std::cout << std::is_lvalue_reference_v<int*&> << std::endl;
	//std::cout << std::is_pointer_v         <int*&> << std::endl;
	//std::cout << std::is_pointer_v         <std::remove_reference_t<int*&>> << std::endl;
//	return 0;

	//int cc = 5;
	//int& ccc = cc;
	//int ttt = 6;
	//std::reference_wrapper<int> rcc(ccc) ;
	//std::cout << rcc.get();
	//rcc = ttt;
	//std::cout << rcc.get();

	//std::cout << simple(std::reference_wrapper<int>(cc));
	//CC<int, int, int>::MakeArgsTuple(std::index_sequence_for<int, int, int>());
	//int s = 10;
	//int** ss = new int* ();
	//*ss = &s;
	//delete ss;

	//TCppFunction<int, int> Function1("simple", &simple);
	//return 0;
	int a = 2;
	int b = 4;

	TCppFunction<int, int, int&> Function("add", &add);
	auto Frame = Function.NewFrame();
	int* pa = (int*)Frame->GetParameter(0);
	int* pb = (int*)Frame->GetParameter(1);
	*pa = 10;
	*pb = 14;

	//Function.SetParameter(Frame.get(), 0ULL, a);
	//Function.SetParameter(Frame.get(), 1ULL, &b);
	Function.Invoke(Frame.get());
	int* pc = (int*)Frame->GetParameter(2);
	std::cout << *pc << std::endl;
	//std::cout << Function.GetParameter<int>(Frame.get(), 2ULL) << std::endl;
	return 0;
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