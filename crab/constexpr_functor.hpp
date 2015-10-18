#ifndef CONSTEXPR_FUNCTOR
#define CONSTEXPR_FUNCTOR

#define CRAB_CONSTEXPR_FUNCTOR(Name, Body)            \
    struct Name                                       \
    {                                                 \
    	constexpr Name() = default;                   \
                                                      \
    	template<typename... Args>                    \
    	constexpr auto operator()(Args... args) const \
    	{                                             \
    		return Body;                              \
    	}                                             \
    }

#define CRAB_FIRST_CLASS_TEMPLATE_FUNCTION(Name, Function) \
    constexpr CRAB_CONSTEXPR_FUNCTOR(__##Name, (Function(args...))) Name = __##Name{};

#endif /* CONSTEXPR_FUNCTOR */