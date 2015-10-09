/* Tested with GCC@9cd04d8b99181eb10980d1c6a009c9ff31d20d40
 *
 * COMPILE_OPTIONS: -std=c++1z
 * RESULT: SUCCESS
 */

/*
 * VARIANTS: EXPECTED_ID_EXPRESSION_BEFORE_STAR_GCC_AT_9cd04d8b99181eb10980d1c6a009c9ff31d20d40_ERROR:FAIL 
 * VARIANTS: P3_ERROR:FAIL
 * VARIANTS: F_GCC_AT_9cd04d8b99181eb10980d1c6a009c9ff31d20d40_ICE:FAIL
 */

struct S1 { int n; };
struct S2 { char c; };
struct S3 { struct X { using Y = int; }; };

template<typename T> concept bool C = sizeof(T) == sizeof(int);

#ifdef F_GCC_AT_9cd04d8b99181eb10980d1c6a009c9ff31d20d40_ICE
void f(typename auto::X::Y){}
#endif

template<int N> concept bool D = true;

int main()
{
#ifdef EXPECTED_ID_EXPRESSION_BEFORE_STAR_GCC_AT_9cd04d8b99181eb10980d1c6a009c9ff31d20d40_ERROR
    auto::* p1 = &S1::n; // auto deduced as S1
    C::* p2 = &S1::n; // OK: C deduced as S1
#endif

#ifdef P3_ERROR
    C::* p3 = &S1::c; // error: deduction fails because constrints are not satisfied
#endif


#ifdef F_GCC_AT_9cd04d8b99181eb10980d1c6a009c9ff31d20d40_ICE
    f(S1{}); // error: auto cannot be deduced from S1
#endif

#ifdef FOO
    f<S3>(0); // OK
#endif

#if P4_ERROR
    D::* p4 = &S1::n; // error: D does not designate a placeholder type
#endif
}
