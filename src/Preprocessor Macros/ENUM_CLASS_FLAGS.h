#include <type_traits>

// Define bitwise operators for an enum class, allowing usage as bitmasks.
#define ENUM_CLASS_FLAGS(Enum)                                      \
    inline constexpr Enum operator|(Enum Lhs, Enum Rhs) {           \
        return static_cast<Enum>(                                   \
            static_cast<uint>(Lhs) |                                \
            static_cast<uint>(Rhs));                                \
    }                                                               \
    inline constexpr Enum operator&(Enum Lhs, Enum Rhs) {           \
        return static_cast<Enum>(                                   \
            static_cast<uint>(Lhs) &                                \
            static_cast<uint>(Rhs));                                \
    }                                                               \
    inline constexpr Enum operator^(Enum Lhs, Enum Rhs) {           \
        return static_cast<Enum>(                                   \
            static_cast<uint>(Lhs) ^                                \
            static_cast<uint>(Rhs));                                \
    }                                                               \
    inline constexpr Enum operator~(Enum E) {                       \
        return static_cast<Enum>(                                   \
            ~static_cast<uint>(E));                                 \
    }                                                               \
    inline constexpr bool operator>(Enum Lhs, Enum Rhs) {           \
        return static_cast<uint>(Lhs) > static_cast<uint>(Rhs);     \
    }                                                               \
    inline constexpr bool operator>(Enum Lhs, uint Rhs) {           \
        return static_cast<uint>(Lhs) > Rhs;                        \
    }                                                               \
    inline Enum& operator|=(Enum& Lhs, Enum Rhs) {                  \
        return Lhs = static_cast<Enum>(                             \
                   static_cast<uint>(Lhs) |                         \
                   static_cast<uint>(Lhs));                         \
    }                                                               \
    inline Enum& operator&=(Enum& Lhs, Enum Rhs) {                  \
        return Lhs = static_cast<Enum>(                             \
                   static_cast<uint>(Lhs) &                         \
                   static_cast<uint>(Lhs));                         \
    }                                                               \
    inline Enum& operator^=(Enum& Lhs, Enum Rhs) {                  \
        return Lhs = static_cast<Enum>(                             \
                   static_cast<uint>(Lhs) ^                         \
                   static_cast<uint>(Lhs));                         \
    }