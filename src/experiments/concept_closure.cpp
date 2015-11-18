
template<typename T>
using void_t = void;

template<typename C, typename... Ts>
concept bool Models = requires(C c)
{
	typename C::template apply<Ts...>;
};

struct Concept
{
	template<typename T> 
	requires requires(T a)
	{
		{a == a};
	}
	struct apply {};
};

#define REMOVE_PARENS(...) __VA_ARGS__ 

#define CONCEPT(Name, Args, Requires) \
struct Name  \
{            \
    template<REMOVE_PARENS Args> \
    requires REMOVE_PARENS Requires \
    struct apply {};  \
}

template<typename T>
struct ComparableWith
{
	CONCEPT(EqualityComparable, 
		(typename U),(
		requires(T a, U b)
		{
			{a == b} -> bool;
		}
	));

	CONCEPT(LessThanComparable, 
		(typename U),(
		requires(T a, U b)
		{
			{a < b} -> bool;
		}
	));
};

CONCEPT(Addable,
	(typename A, typename B, typename... Args),(
	requires(A a, B b, Args... args)
	{
		{a + b};
		requires Models<Addable, decltype(a + b), Args...>;
	}
));

template<typename Concept, typename T>
	requires Models<Concept,T>
bool f(T a)
{
	return a == a;
}

template<typename... Args>
	requires Models<Addable, Args...>
void g(Args... args)
{}

int main()
{
	f<ComparableWith<int>::LessThanComparable>(1);

	g(1u, 2, 3.0f, 4.0);
}
