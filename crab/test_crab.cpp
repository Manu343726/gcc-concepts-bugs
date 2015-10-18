
#include "crab.hpp"
#include <iostream>

#define CRAB_TEST(...) std::cout << #__VA_ARGS__ " --> " << std::boolalpha << (__VA_ARGS__.value()) << std::endl

using namespace crab;

int main()
{
	CRAB_TEST(Switch());
	CRAB_TEST(Switch().Case(true).Then(false));

	CRAB_TEST(Switch().Case(false).Then(true)
		              .Case(true).Then(false));

	CRAB_TEST(Switch().Case(false).Then(true)
		              .Case(true).Then(false)
					  .Default().Then(true));

	CRAB_TEST(Switch().Case(true).Then(true));
	CRAB_TEST(Switch().Case(true).Then(true)
		              .Case(false).Then(false));
	CRAB_TEST(Switch().Case(true).Then(true)
		              .Case(false).Then(false)
					  .Default().Then(false));
}