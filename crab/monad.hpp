#ifndef MONAD_HPP
#define MONAD_HPP

#include <type_traits>

namespace crab
{
	template<typename Monad, typename... Args>
	constexpr Monad Return(Args... args)
	{
		return Monad{typename Monad::value_type{args...}};
	}

	template<typename Monad, typename F>
	constexpr Monad operator>>=(Monad m, F f)
	{
		return m.Bind(f);
	}

	namespace datatypes
	{
		template<typename T, typename ValueType>
		struct Monad
		{
			using value_type = ValueType;

			template<typename F>
			constexpr T Bind(F function)
			{
				return Return<T>(function(static_cast<const T&>(*this).value()));
			}
		};
	}
}

#endif /* MONAD_HPP */