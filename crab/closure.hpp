#ifndef CLOSURE_HPP
#define CLOSURE_HPP

#include "tuple.hpp"

namespace crab
{
	namespace keywords
	{
		struct InitializeWithTuple {};
	}

	namespace datatypes
	{
		

		template<typename F, typename... Args>
		struct Closure
		{
			constexpr Closure(F f, Args... args) :
				_f{f},
				_args{args...}
			{}

			constexpr Closure(F f, ::crab::keywords::InitializeWithTuple, Tuple<Args...> args) :
				_f{f},
				_args{args}
			{}

			template<typename... FollowingArgs>
			constexpr auto operator()(FollowingArgs... following) const
			{
				return tuple_call(_f, _args, following...);
			}

			constexpr F function() const
			{
				return _f;
			}

			constexpr Tuple<Args...> captured_args() const
			{
				return _args;
			}

		private:
			F _f;
			Tuple<Args...> _args;
		};

		template<typename F>
		struct Closure<F>
		{
			constexpr Closure(F f) :
				_f{f}
			{}

			template<typename... FollowingArgs>
			constexpr auto operator()(FollowingArgs... following) const
			{
				return _f(following...);
			}

			constexpr F function() const
			{
				return _f;
			}

			constexpr Tuple<> captured_args() const
			{
				return ::crab::make_tuple();
			}

		private:
			F _f;
		};
	}

	template<typename F, typename... Args>
	constexpr datatypes::Closure<F, Args...> closure(F f, Args... args)
	{
		return {f, args...};
	}
}

#endif /* CLOSURE_HPP */