#ifndef CRAB_HPP

namespace crab
{
	namespace types
	{
		class Boolean
		{
		public:
			constexpr Boolean(bool value) :
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
		private:
			bool _value;
		};

		template<typename Switch>
		class SwitchCase
		{
		public:
			constexpr SwitchCase(Switch parent, bool when) :
				_switch{parent},
				_when{when}
			{}


			struct SwitchCaseThen : public Boolean
			{
				using Boolean::Boolean;

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

		class Switch : public Boolean
		{
		public:
			using Boolean::Boolean;

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

#endif /* CRAB_HPP */