#ifndef SWITCH_HPP
#define SWITCH_HPP

#include "boolean.hpp"

namespace crab
{
	namespace types
	{
		template<typename Switch>
		class SwitchCase
		{
		public:
			constexpr SwitchCase(Switch parent, bool when) :
				_switch{parent},
				_when{when}
			{}


			struct SwitchCaseThen : public ::crab::datatypes::BooleanInterface<SwitchCaseThen>
			{
				using ::crab::datatypes::BooleanInterface<SwitchCaseThen>::BooleanInterface;

				constexpr SwitchCase operator()(bool then) const
				{
					return {!(_when && !then)};
				}
			};

			constexpr Switch Then(bool then) const
			{
				return {_switch.value() || (_when && then)};
			}

			constexpr SwitchCase operator--() const
			{
				return *this;
			}

			constexpr SwitchCase operator>(bool then) const
			{
				return Then(then);
			}

			constexpr SwitchCaseThen operator->() const
			{
				return {_when};
			}

		private:
			bool _when;
			Switch _switch;
		};

		class Switch : public ::crab::datatypes::BooleanInterface<Switch>
		{
		public:
			using ::crab::datatypes::BooleanInterface<Switch>::BooleanInterface;

			constexpr SwitchCase<Switch> Case(bool when) const
			{
				return {*this, when};
			}

			constexpr SwitchCase<Switch> Default() const
			{
				return Case(true);
			}
		};
	}

	constexpr ::crab::types::Switch Switch()
	{
		return {false};
	}
}

#endif /* SWITCH_HPP */