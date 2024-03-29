#pragma once
#define FIRST(a, ...) a
#define SECOND(a, b, ...) b

#define EMPTY()
#define EVAL(...) EVAL1024(__VA_ARGS__)
#define EVAL1024(...) EVAL512(EVAL512(__VA_ARGS__))
#define EVAL512(...) EVAL256(EVAL256(__VA_ARGS__))
#define EVAL256(...) EVAL128(EVAL128(__VA_ARGS__))
#define EVAL128(...) EVAL64(EVAL64(__VA_ARGS__))
#define EVAL64(...) EVAL32(EVAL32(__VA_ARGS__))
#define EVAL32(...) EVAL16(EVAL16(__VA_ARGS__))
#define EVAL16(...) EVAL8(EVAL8(__VA_ARGS__))
#define EVAL8(...) EVAL4(EVAL4(__VA_ARGS__))
#define EVAL4(...) EVAL2(EVAL2(__VA_ARGS__))
#define EVAL2(...) EVAL1(EVAL1(__VA_ARGS__))
#define EVAL1(...) __VA_ARGS__
#define DEFER2(m) m EMPTY EMPTY()()

#define IS_PROBE(...) SECOND(__VA_ARGS__, 0)
#define PROBE() ~, 1

#define CAT(a,b) a ## b

#define NOT(x) IS_PROBE(CAT(_NOT_, x))
#define _NOT_0 PROBE()

#define BOOL(x) NOT(NOT(x))

#define IF_ELSE(condition) _IF_ELSE(BOOL(condition))
#define _IF_ELSE(condition) CAT(_IF_, condition)

#define _IF_1(...) __VA_ARGS__ _IF_1_ELSE
#define _IF_0(...)             _IF_0_ELSE

#define _IF_1_ELSE(...)
#define _IF_0_ELSE(...) __VA_ARGS__

#define HAS_ARGS(...) BOOL(FIRST(_END_OF_ARGUMENTS_ __VA_ARGS__)())
#define _END_OF_ARGUMENTS_() 0

#define MAP(m, first, ...)           \
  m(first)                           \
  IF_ELSE(HAS_ARGS(__VA_ARGS__))(    \
    DEFER2(_MAP)()(m, __VA_ARGS__)   \
  )(                                 \
    /* Do nothing, just terminate */ \
  )
#define _MAP() MAP

#define GET_ITEM(O,P) \
static inline \
communication::datatypes::P* get_ ## O(O ## _t &x) { \
    if (x.obj == nullptr) \
        return nullptr; \
    return static_cast<communication::datatypes::P*>(x.obj); \
}

#define GET_ITEMP(O,P) \
static inline \
communication::datatypes::P* get_ ## O(O ## _t* x) { \
    if (x == NULL) \
        return nullptr; \
    if (x->obj == nullptr) \
        return nullptr; \
    return static_cast<communication::datatypes::P*>(x->obj); \
}

#define GET_ITEMC(O,P) \
static inline \
communication::datatypes::P* get_ ## O(const O ## _t &x) { \
    if (x.obj == nullptr) \
        return nullptr; \
    return static_cast<communication::datatypes::P*>(x.obj); \
}

#define GET_ITEMCP(O,P) \
static inline \
communication::datatypes::P* get_ ## O(const O ## _t* x) { \
    if (x == NULL) \
        return nullptr; \
    if (x->obj == nullptr) \
        return nullptr; \
    return static_cast<communication::datatypes::P*>(x->obj); \
}
