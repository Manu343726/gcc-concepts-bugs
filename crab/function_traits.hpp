#ifndef FUNCTION_TRAITS_HPP
#define FUNCTION_TRAITS_HPP

#include <tuple>

namespace crab
{
	namespace detail
	{
		template<typename...>
		using void_t = void;

		struct function_constants
		{
			struct UnknownType{};
			static constexpr int UnknownArity = -1;
			static constexpr int VariadicArity = -2;
		};

		template<typename F>
		struct function_traits
		{
			static constexpr int arity = function_constants::UnknownArity;
			using return_type = function_constants::UnknownType;
			using arg_types = function_constants::UnknownType;
		};

		template<typename R, typename... Args>
		struct function_traits<R(Args...)>
		{
			static constexpr int arity = sizeof...(Args);
			using return_type = R;
			using arg_types = std::tuple<Args...>;
		};

		template<typename R, typename... Args>
		struct function_traits<R(*)(Args...)>
		{
			static constexpr int arity = sizeof...(Args);
			using return_type = R;
			using arg_types = std::tuple<Args...>;
		};

		template<typename R, typename C, typename... Args>
		struct function_traits<R(C::*)(Args...)>
		{
			static constexpr int arity = sizeof...(Args);
			using return_type = R;
			using arg_types = std::tuple<Args...>;
		};
	}
}

#endif