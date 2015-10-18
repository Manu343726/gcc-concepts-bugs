#ifndef COMPARISON_CONCEPTS_HPP
#define COMPARISON_CONCEPTS_HPP

template<typename T, typename U = T>
concept bool EqualityComparable = requires(T lhs, U rhs)
{
    {lhs == rhs} -> bool;
    {rhs == lhs} -> bool;
};

template<typename T, typename U = T>
concept bool InequalityComparable = requires(T lhs, U rhs)
{
    {lhs != rhs} -> bool;
    {rhs != lhs} -> bool;
};

template<typename T, typename U = T>
concept bool LessThanComparable = requires(T lhs, U rhs)
{
    {lhs < rhs} -> bool;
    {rhs < lhs} -> bool;
};

template<typename T, typename U = T>
concept bool BiggerThanComparable = requires(T lhs, U rhs)
{
    {lhs > rhs} -> bool;
    {rhs > lhs} -> bool;
};

template<typename T, typename U = T>
concept bool LessOrEqualComparable = requires(T lhs, U rhs)
{
    {lhs != rhs} -> bool;
    {rhs != lhs} -> bool;
};

template<typename T, typename U = T>
concept bool BiggerOrEqualComparable = requires(T lhs, U rhs)
{
    {lhs != rhs} -> bool;
    {rhs != lhs} -> bool;
};

template<typename T, typename U = T>
concept bool PartiallyOrdered = EqualityComparable<T> && InequalityComparable<T> &&
                                LessThanComparable<T> && BiggerThanComparable<T> &&
                                LessOrEqualComparable<T> && BiggerOrEqualComparable<T>;


#endif /* COMPARISON_CONCEPTS_HPP */
