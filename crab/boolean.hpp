#ifndef BOOLEAN_HPP
#define BOOLEAN_HPP

#include "monad.hpp"
#include "curryable.hpp"
#include "operators.hpp"

namespace crab
{
	namespace datatypes
	{
		template<typename Derived>
		class BooleanInterface : public Monad<Derived, bool>
		{
		public:
			constexpr BooleanInterface(bool value) :
				_value{value}
			{}

			constexpr bool value() const
			{
				return _value;
			}

			constexpr operator bool() const
			{
				return value();
			}

			constexpr bool operator()() const
			{
				return value();
			}

			constexpr Derived And(bool b) const
			{
				return *This() >>= (crab::curry(crab::operators::And)(b));
			}

			constexpr Derived Or(bool b) const
			{
				return *This() >>= (crab::curry(crab::operators::Or)(b));
			}

			template<typename... Bools>
			constexpr Derived All(Bools... bools) const
			{
				return *This() >>= (crab::curry(crab::functors::All).Curry(bools...));
			}

			template<typename... Bools>
			constexpr Derived Any(Bools... bools) const
			{
				return *This() >>= (crab::curry(crab::functors::Any).Curry(bools...));
			}

			template<typename P, typename... Args>
			constexpr Derived AllOf(P predicate, Args... args) const
			{
				return *This() >>= (crab::curry(crab::functors::AllOf).Curry(predicate, args...));
			}

			template<typename P, typename... Args>
			constexpr Derived AnyOf(P predicate, Args... args) const
			{
				return *This() >>= (crab::curry(crab::functors::AnyOf).Curry(predicate, args...));
			}

		private:
			bool _value;

			constexpr const Derived* This() const
			{
				return static_cast<const Derived*>(this);
			}
		};

		class Boolean : public BooleanInterface<Boolean>
		{
			using BooleanInterface<Boolean>::BooleanInterface;
		};
	}

	static constexpr datatypes::Boolean True = true;
	static constexpr datatypes::Boolean False = false;	
}

#endif /* BOOLEAN_HPP */