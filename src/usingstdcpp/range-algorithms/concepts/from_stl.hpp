
#ifndef FROM_STL_HPP
#define FROM_STL_HPP

#include <type_traits>

#define CONCEPT_FROM_TRAIT(Concept, Trait)  \
    template<typename... Ts>                \
    concept bool Concept = Trait<Ts...>::value

CONCEPT_FROM_TRAIT(DefaultConstructible, std::is_default_constructible);
CONCEPT_FROM_TRAIT(CopyConstructible, std::is_copy_constructible);
CONCEPT_FROM_TRAIT(CopyAssignable, std::is_copy_assignable);
CONCEPT_FROM_TRAIT(Destructible, std::is_destructible);
CONCEPT_FROM_TRAIT(Same, std::is_same);

#endif /* FROM_STL_HPP */
