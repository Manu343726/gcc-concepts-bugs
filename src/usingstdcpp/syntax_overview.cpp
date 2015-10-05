/*
 * COMPILE_OPTIONS: -std=c++1z -Wall -Werror
 */

template<typename T>
concept bool UnaryConcept() {
	return sizeof(T) > 0;
}

template<typename T, typename U>
concept bool BinaryConcept() {
	return sizeof(T) > 0 && sizeof(U) > 0;
}

template<typename T>
concept bool AsVariableTemplateConcept = sizeof(T) > 0;

template<typename T>
concept bool InvokingAnotherConcept = UnaryConcept<T>();

template<typename T>
concept bool RequiresClause() {
	return requires(T a, T b) {
		{a == b},
		{a == b} -> bool,
		{a.f()},
		{f(a)},
		typename T::member_type,
		template T::member_template,
		{f(a)} -> UnaryConcept
	}
}

struct PassesRequiresClauseExample {
	friend bool operator==(const PassesRequiresClauseExample&, const PassesRequiresClauseExample&) {
		return true;
	}

	friend bool operator!=(const PassesRequiresClauseExample&, const PassesRequiresClauseExample&) {
		return false;
	}

	void f() {}

	struct member_type {};

	template<typename...> struct member_template {};
};

int f(const PassesRequiresClauseExample&) {
	return 0;
}

int main() {
	[](RequiresClause){}(PassesRequiresClauseExample{});
}
