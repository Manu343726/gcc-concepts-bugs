
#include "tuple.hpp"
#include "curryable.hpp"

#include <iostream>

constexpr int add(int a, int b, int c) 
{
	return a + b + c;
}

static_assert(crab::detail::function_traits<decltype(add)>::arity == 3, "");

constexpr auto t = crab::make_tuple();

constexpr auto curried = crab::curry(add);
constexpr auto c1 = curried(1,2);
constexpr int result = c1(3);

int main()
{
	constexpr auto t = crab::make_tuple(1,2, 3);

	static_assert(std::is_same<int, decltype(t.get<int>())>::value, "");
	static_assert(crab::tuple_call(add, t) == 6, "");

	std::cout << std::boolalpha;
	std::cout << result << std::endl;
	std::cout << "(" << t.get<0>() << "," << t.get<1>() << "," << t.get<2>() << ")" << std::endl;
}