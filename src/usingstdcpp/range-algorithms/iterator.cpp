
#include "concepts/iterator.hpp"

#include <vector>
#include <algorithm>
#include <iostream>

RandomAccessIterator{It}
void print_range(It begin, It end)
{
	std::copy(begin, end, std::ostream_iterator<int>(std::cout, " "));
}



int main()
{
	std::vector<int> v = {1, 2, 3, 4};

	using It = decltype(v.begin());

	[](Iterator){}(v.begin());
	[](InputIterator){}(v.begin());
	[](OutputIterator<detail::value_type<It>>){}(v.begin());
	[](ForwardIterator){}(v.begin());
	[](BidirectionalIterator){}(v.begin());	

	print_range(v.begin(), v.end());
}