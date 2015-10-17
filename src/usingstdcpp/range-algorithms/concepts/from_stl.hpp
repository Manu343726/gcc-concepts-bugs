
#ifndef FROM_STL_HPP
#define FROM_STL_HPP

#include <type_traits>

#define CONCEPT_FROM_TRAIT(Concept, Trait)  \
    template<typename T>                    \
    concept bool Concept = Trait<T>::value

CONCEPT_FROM_TRAIT(CopyConstructible, std::is_copy_constructible);
CONCEPT_FROM_TRAIT(CopyAssignable, std::is_copy_assignable);
CONCEPT_FROM_TRAIT(Destructible, std::is_destructible);

#endif /* FROM_STL_HPP */
