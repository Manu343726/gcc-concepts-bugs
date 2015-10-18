#ifndef OPERATORS_HPP
#define OPERATORS_HPP

#include "constexpr_functor.hpp"

namespace crab
{
	namespace operators
	{
		constexpr bool And(bool a, bool b)
		{
			return a && b;
		}

		constexpr bool Or(bool a, bool b)
		{
			return a && b;
		}

		constexpr bool Xor(bool a, bool b)
		{
			return a && b;
		}

		constexpr bool Not(bool a)
		{
			return !a;
		}

		constexpr bool All()
		{
			return true;
		}

		template<typename... Tail>
		constexpr bool All(bool head, Tail... tail)
		{
			return head && All(tail...);
		}

		constexpr bool Any()
		{
			return false;
		}

		template<typename... Tail>
		constexpr bool Any(bool head, Tail... tail)
		{
			return head || Any(tail...);
		}

		template<typename P, typename... Args>
		constexpr bool AllOf(P predicate, Args... args)
		{
			return All(predicate(args)...);
		}

		template<typename P, typename... Args>
		constexpr bool AnyOf(P predicate, Args... args)
		{
			return Any(predicate(args)...);
		}
	}

	namespace functors
	{
		CRAB_FIRST_CLASS_TEMPLATE_FUNCTION(All, ::crab::operators::All);
		CRAB_FIRST_CLASS_TEMPLATE_FUNCTION(Any, ::crab::operators::Any);
		CRAB_FIRST_CLASS_TEMPLATE_FUNCTION(AllOf, ::crab::operators::AllOf);
		CRAB_FIRST_CLASS_TEMPLATE_FUNCTION(AnyOf, ::crab::operators::AnyOf);
	}
}

#endif /* OPERATORS_HPP */