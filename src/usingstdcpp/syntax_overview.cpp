/*
 * COMPILE_OPTIONS: -std=c++1z -Wall -Werror
 */

template<typename T>
concept bool UnaryConcept() {
	return sizeof(T) > 0; // Return a constant expression
}

template<typename T, typename U>
concept bool BinaryConcept() {
	return sizeof(T) > 0 && sizeof(U) > 0; // Of course logical operators are supported
}

template<typename T>
concept bool AsVariableTemplateConcept = sizeof(T) > 0; 

template<typename T>
concept bool InvokingAnotherConcept = UnaryConcept<T>(); // You can evaluate other concepts

template<typename T>
concept bool RequiresClause() {
	return requires(T a, T b) {
		a == b;           // a == b is valid syntax (i.e. exists operator==(T,T))
		{a == b} -> bool; // a == b is valid syntax and returns bool
		a.f();            // T has f() member function
		f(a);             // there's a f(T) non-member function
		typename T::member_type;     // T has "member_type" member type
		template T::member_template; // T has "member_template" template
		{f(a)} -> UnaryConcept // There's f(T) function and yields a type 
		                       // that meets UnaryConcept concept
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

// Note int meets UnaryConcept
int f(const PassesRequiresClauseExample&) {
	return 0;
}

int main() {
	[](RequiresClause){}(PassesRequiresClauseExample{});
}
