#ifndef SWAPPABLE_HPP
#define SWAPPABLE_HPP

#include <utility>

template<typename T>
concept bool Swappable = requires(T& lvalue_ref)
{
    swap(lvalue_ref, lvalue_ref);
};

#endif /* SWAPPABLE_HPP */
