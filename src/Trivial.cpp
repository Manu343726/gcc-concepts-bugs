/* DESCRIPTION: Check Trivial() concept (Yields true for any T)
 * SOURCES: Trivial.cpp
 * INCLUDE_DIRECTORIES: . ..
 * COMPILE_OPTIONS: -std=c++1z -Wall
 * RESULT: EXPECTED_SUCCESS
 */

#include "utils.hpp"
#include "concepts/trivial.hpp"

#include <vector>

int main()
{
    CONCEPT_ASSERT(Trivial,int{});
    CONCEPT_ASSERT(Trivial,bool{});
    CONCEPT_ASSERT(Trivial,std::vector<int>{});
    //... Here's where I miss reflection, imagine a loop over C++ types
    //without having to write a typelist by hand
}