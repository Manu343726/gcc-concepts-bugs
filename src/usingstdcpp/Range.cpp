/*
 * COMPILE_OPTIONS: -std=c++1z
 * RESULT: FAILS__GCC_BUG_COMMIT_9cd04d8b99181eb10980d1c6a009c9ff31d20d40
 * SKIP: TRUE
 */

#include <type_traits>

template<typename T, typename U = T>
concept bool EqualityComparable = requires(T a, T b) {
    a == b;
    a != b;
};

template<typename T>
concept bool MoveConstructible = std::is_move_constructible<T>::value;

template<typename T>
concept bool CopyConstructible = std::is_copy_constructible<T>::value;

template<typename T>
concept bool CopyAssignable = std::is_copy_assignable<T>::value;

template<typename T>
concept bool Destructible = std::is_constructible<T>::value;

template<typename T>
concept bool Swapabble = requires(T a, T b) {
    swap(a, b);
    swap(b, a);
};

template<typename It>
concept bool Iterator = CopyConstructible<It> && CopyAssignable<It> && Destructible<It> && Swapabble<It> &&
    requires(It it) {
        *it;
        {++it} -> It&;
};

template<typename It>
concept bool ForwardIterator = EqualityComparable<It> &&    
    requires(It it) {
        ++it;
        *it;
};

template<typename It>
concept bool BackwardIterator = EqualityComparable<It> &&
    requires(It it) {
        --it;
        *it;
};

template<typename It>
concept bool BidirectionalIterator = ForwardIterator<It> && BackwardIterator<It>;


template<typename It>
concept bool RandomAccessIterator = BidirectionalIterator<It> && 
    requires(It it) {
        it + 42;
        it - 666;
};

template<typename R>
concept bool Range = requires(R range) {
    {begin(range)} -> Iterator;
    {end(range)} -> Iterator;
};

#include <vector>

int main() {
    int array[666];

    [](Range){}(std::vector<int>{});
}
