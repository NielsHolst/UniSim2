#ifndef BASE_ENUM_FUNCTIONS_H
#define BASE_ENUM_FUNCTIONS_H

#define DEFINE_ENUM_FUNCTIONS(CLASS) \
constexpr CLASS operator|(CLASS a, CLASS b) { \
    return static_cast<CLASS> \
        (static_cast<int>(a) | static_cast<int>(b)); \
} \
\
constexpr CLASS operator&(CLASS a, CLASS b) { \
    return static_cast<CLASS> \
        (static_cast<int>(a) & static_cast<int>(b)); \
} \
\
constexpr bool operator||(CLASS a, CLASS b) { \
    return static_cast<bool> (a | b);\
} \
\
constexpr bool operator&&(CLASS a, CLASS b) { \
    return static_cast<bool>(a & b); \
}

#endif

