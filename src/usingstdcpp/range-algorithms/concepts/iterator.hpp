#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include "from_stl.hpp"
#include "swappable.hpp"
#include "comparison.hpp"
#include "crab/crab.hpp"

#include <iterator>

namespace detail
{
    template<typename It>
    using value_type = typename std::iterator_traits<It>::value_type;

    template<typename It>
    using reference = typename std::iterator_traits<It>::reference;

    template<typename It>
    using difference_type = typename std::iterator_traits<It>::difference_type;
}

template<typename It>
concept bool Iterator = requires(It lvalue)
{
    requires CopyConstructible<It>;
    requires CopyAssignable<It>;
    requires Destructible<It>;

    requires Swappable<It>;

    *lvalue;
    {++lvalue} -> It&
};

template<typename It>
concept bool InputIterator = requires(It i, It j)
{
    requires Iterator<It>;
    requires EqualityComparable<It>;

    typename detail::value_type<It>;
    typename detail::reference<It>;

    {i != j} -> bool;
    {*i} -> typename detail::reference<It>;
    {*i++} -> typename detail::value_type<It>;
};

template<typename It, typename OutputType = detail::value_type<It>>
concept bool OutputIterator = requires(It lvalue, OutputType output)
{
    requires Iterator<It>;
    requires std::is_pointer<It>::value || std::is_class<It>::value;
    
    {lvalue++} -> const It&;

    *lvalue = output;
    *lvalue++ = output;
};

template<typename It>
concept bool ForwardIterator = requires(It i)
{
    requires InputIterator<It>;
    requires DefaultConstructible<It>;

    requires crab::Switch()
        .Case(OutputIterator<It, detail::value_type<It>>)
           .Then(Same<detail::value_type<It>&, detail::reference<It>>)
        .Default()
           .Then(Same<const detail::value_type<It>&, detail::reference<It>>)
    ();

    {i++} -> It;
    {*i++} -> detail::reference<It>;
};

template<typename It>
concept bool BidirectionalIterator = requires(It a, It b)
{
    requires ForwardIterator<It>;

    {--a} -> It&;
    {a--} -> const It&;
    {*a--} -> detail::reference<It>;
};

template<typename It>
concept bool RandomAccessIterator = requires(It i, It a, It b, It& r, detail::difference_type<It> n)
{
    requires BidirectionalIterator<It>; 

    {r += n} -> It&;
    {a + n} -> It;
    {n + a } -> It;
    {r -= n} -> It&;
    {i - n} -> It;
    {b - a} -> detail::difference_type<It>;
    {i[n]} -> detail::reference<It>;
    
    requires LessThanComparable<It>;
};

#endif /* ITERATOR_HPP */
