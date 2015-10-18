#ifndef CURRYABLE_HPP
#define CURRYABLE_HPP

#include "constexpr_functor.hpp"
#include "closure.hpp"
#include "function_traits.hpp"

namespace crab
{
	namespace keywords
	{
		static constexpr struct CurryMark {} Curry;
	}

	namespace datatypes
	{
		template<typename F, typename... Args>
		struct Curryable : public Closure<F, Args...>
		{
		public:
			using base = Closure<F, Args...>;

			using base::Closure;

			template<typename... MoreArgs>
			constexpr auto operator()(MoreArgs... args) const
			{
				//return tuple_call(base::function(), base::captured_args(), args...);
				return invoke<typelist<MoreArgs...>, false>::apply(*this, args...);
			}

			template<typename... MoreArgs>
			constexpr auto operator()(::crab::keywords::CurryMark, MoreArgs... args) const
			{
				//return tuple_call(base::function(), base::captured_args(), args...);
				return invoke<typelist<MoreArgs...>, true>::apply(*this, args...);
			}

			template<typename... MoreArgs>
			constexpr auto Curry(MoreArgs... args) const
			{
				return Curryable<F, Args..., MoreArgs...>{base::function(), ::crab::keywords::InitializeWithTuple{}, tuple_cat(base::captured_args(), ::crab::make_tuple(args...))};
			}

		private:
			template<typename... Ts>
			struct typelist
			{
				static constexpr std::size_t size = sizeof...(Ts);
			};

			template<typename MoreArgs, bool BreakMarker, bool CompleteCall = ((::crab::detail::function_traits<F>::arity < 0) || ::crab::detail::function_traits<F>::arity == MoreArgs::size + sizeof...(Args))>
			struct invoke;

			template<typename... MoreArgs>
			struct invoke<typelist<MoreArgs...>,false, true>
			{
				template<typename... Ts>
				static constexpr auto apply(Curryable<F,Args...> This, MoreArgs... args)
				{
					return tuple_call(This.function(), This.captured_args(), args...);
				}
			};

			template<typename... MoreArgs>
			struct invoke<typelist<MoreArgs...>, false, false>
			{
				template<typename... Ts>
				static constexpr auto apply(Curryable<F,Args...> This, MoreArgs... args)
				{
					return This.Curry(args...);
				}	
			};

			template<typename... MoreArgs, bool CompleteCall>
			struct invoke<typelist<MoreArgs...>, true, CompleteCall>
			{
				template<typename... Ts>
				static constexpr auto apply(Curryable<F,Args...> This, MoreArgs... args)
				{
					return This.Curry(args...);
				}	
			};
		};
	}

	template<typename Function>
	constexpr datatypes::Curryable<Function> curry(Function f)
	{
		return {f};
	}
}

#endif /* CURRYABLE_HPP */