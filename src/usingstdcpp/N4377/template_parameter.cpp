/*
 * COMPILE_OPTIONS: -std=c++1z
 * RESULT: N4377::14.1_TEMPLATE_PARAMETERS
 * VARIANTS: DEFAULT CONCEPT_C4_GCC_AT_9cd04d8b99181eb10980d1c6a009c9ff31d20d40_BUG:FAIL
 */

template<typename T> concept bool C1 = true;
template<template<typename> class X> concept bool C2 = true;
template<int N> concept bool C3 = true;
template<typename... Ts> concept bool C4 = true;
template<char... Cs> concept bool C5 = true;

template<C1 T> void f1(){}
template<C2 X> void f2(){}
template<C3 N> void f3(){}
template<C4... Ts> void f4(){}
#ifdef CONCEPT_C4_GCC_AT_9cd04d8b99181eb10980d1c6a009c9ff31d20d40_BUG
// GCC@9cd04d8b99181eb10980d1c6a009c9ff31d20d40 error:
// variadic constraint introduced without '...' before '>' token
template<C4 T> void f5(){}
#endif
template<C5... Cs> void f6(){}

struct Type {};
template<typename> struct TemplateWithOneTypeParameter;

int main()
{
    f1<Type>();
    f2<TemplateWithOneTypeParameter>();
    f3<1>();
    f4<Type,Type,Type>();
#ifdef CONCEPT_C4_GCC_AT_9cd04d8b99181eb10980d1c6a009c9ff31d20d40_BUG
    f5<Type>();
#endif
    f6<'p','a','l','m','e','r','i','t','a'>();
}
