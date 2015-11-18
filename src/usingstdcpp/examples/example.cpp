
#include <iostream>
#include <type_traits>
#include <string>
#include <memory>
#include <vector>
#include <string>

#include "src/demangle.hpp"
#include "crab/boolean.hpp"

template<typename T>
concept bool Integral = std::is_integral<T>::value;

template<typename T>
concept bool FloatingPoint = std::is_floating_point<T>::value;

template<typename T>
concept bool Stringizable = requires(const T& value)
{
	{value.to_string()} -> std::string;
};

template<typename T>
concept bool Allocator = requires(const T& alloc)
{
	typename std::allocator_traits<T>;
};

template<typename T>
concept bool RawAllocator = requires(const T& alloc)
{
	requires Allocator<T>;
	alloc.hello_foonathan();
};



void print(Integral i)
{
	std::cout << "Integer! (" << tml::impl::demangle(i) << ") " << i << std::endl;
}

void print(FloatingPoint f)
{
	std::cout << "Floating point! (" << tml::impl::demangle(f) << ") " << f << std::endl;
}

void print(Stringizable object)
{
	std::cout << "Has .to_string() method (member function JD)! " 
	          << "(" << tml::impl::demangle(object) << ") "
	          << object.to_string() << std::endl;
}

struct MyClass
{
	MyClass(int i) :
	    i{i}
	{}

	int i;

	std::string to_string() const
	{
		return "MyClass(" + std::to_string(i) + ")";
	}
};

template<typename T, Allocator Alloc = std::allocator<T>>
struct vector : public std::vector<T, Alloc>
{

};

#ifdef FOO
template<typename T>
struct vector<T, RawAllocator Alloc>
{
   //...
};
#endif


int main()
{
	vector<int> v;

	print(1);
	print(1.0f);
	print(true);
	print(1.0);
	print(MyClass{1});
}

