#ifndef TUPLE_HPP
#define TUPLE_HPP

#include <utility>

namespace crab
{
	namespace datatypes
	{
		namespace detail
		{
			template<typename T>
			struct TupleGetByTypeTag
			{
				constexpr TupleGetByTypeTag() = default;
			};

			template<std::size_t Index>
			struct TupleGetByIndexTag
			{
				constexpr TupleGetByIndexTag() = default;
			};

			template<typename T, std::size_t Index>
			struct TupleElem
			{
				constexpr TupleElem(T elem) :
					elem{elem}
				{}

				constexpr T get(TupleGetByTypeTag<T>) const
				{
					return elem;
				}

				constexpr T get(TupleGetByIndexTag<Index>) const
				{
					return elem;
				}

			private:
				T elem;
			};

			template<typename... Elems>
			struct TupleStorage;

			template<typename Head, typename... Tail>
			struct TupleStorage<Head, Tail...> : public Head, public TupleStorage<Tail...>
			{
				constexpr TupleStorage(Head head, Tail... tail) :
					Head{head}, TupleStorage<Tail...>{tail...}
				{}

				using Head::get;
				using TupleStorage<Tail...>::get;
			};

			template<typename Last>
			struct TupleStorage<Last> : public Last
			{
				constexpr TupleStorage(Last last) :
					Last{last}
				{};

				using Last::get;
			};

			template<>
			struct TupleStorage<>
			{
				struct EmptyTuple {};

				constexpr EmptyTuple get(TupleGetByIndexTag<-1>) const
				{
					return {};
				}

				constexpr EmptyTuple get(TupleGetByTypeTag<EmptyTuple>) const
				{
					return {};
				}	
			};

			template<typename Indices, typename... Ts>
			struct TupleBase;

			template<std::size_t... Indices, typename... Ts>
			struct TupleBase<std::index_sequence<Indices...>, Ts...> : public TupleStorage<TupleElem<Ts, Indices>...>
			{
				constexpr TupleBase(Ts... elems) :
					TupleStorage<TupleElem<Ts, Indices>...>{elems...}
				{}

				using TupleStorage<TupleElem<Ts, Indices>...>::get;
			};

			template<typename... Ts>
			using tuple_base = TupleBase<std::index_sequence_for<Ts...>, Ts...>;


		}

		template<typename... Ts>
		struct Tuple : public detail::tuple_base<Ts...>
		{
			constexpr Tuple(Ts... elems) :
				detail::tuple_base<Ts...>{elems...}
			{}

			template<typename T>
			constexpr T get() const
			{
				return detail::tuple_base<Ts...>::get(detail::TupleGetByTypeTag<T>{});
			}

			template<std::size_t Index>
			constexpr auto get() const
			{
				return detail::tuple_base<Ts...>::get(detail::TupleGetByIndexTag<Index>{});
			}

			static constexpr std::size_t size = sizeof...(Ts);
		};
	}

	template<typename... Ts>
	constexpr datatypes::Tuple<Ts...> make_tuple(Ts... elems)
	{
		return { elems... };
	}

	template<typename T, typename... Ts>
	constexpr T tuple_get(datatypes::Tuple<Ts...> t)
	{
		return t.template get<T>();
	}

	template<std::size_t Index, typename... Ts>
	constexpr auto tuple_get(datatypes::Tuple<Ts...> t)
	{
		return t.template get<Index>();
	}

	namespace detail
	{
		template<typename F, std::size_t... Is, typename... Ts, typename... Args>
		constexpr auto tuple_call(F f, std::index_sequence<Is...>, datatypes::Tuple<Ts...> t, Args... following)
		{
			return f(tuple_get<Is>(t)..., following...);
		}

		template<std::size_t... Is, std::size_t... Js, typename... Ts, typename... Us>
		constexpr auto tuple_cat(std::index_sequence<Is...>, std::index_sequence<Js...>, datatypes::Tuple<Ts...> a, datatypes::Tuple<Us...> b)
		{
			return make_tuple(tuple_get<Is>(a)..., tuple_get<Js>(b)...);
		}
	}

	template<typename F, typename... Ts, typename... Args>
	constexpr auto tuple_call(F f, datatypes::Tuple<Ts...> t, Args... following)
	{
		return detail::tuple_call(f, std::index_sequence_for<Ts...>{}, t, following...);
	}

	template<typename... Ts, typename... Us>
	constexpr datatypes::Tuple<Ts...,Us...> tuple_cat(datatypes::Tuple<Ts...> a, datatypes::Tuple<Us...> b)
	{
		return detail::tuple_cat(std::index_sequence_for<Ts...>{}, std::index_sequence_for<Us...>{},
			                     a, b);
	}
}

#endif /* TUPLE_HPP */