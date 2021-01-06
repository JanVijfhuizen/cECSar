#pragma once
#include <cstdint>
#include <type_traits>
#include <variant>

namespace utils
{

#define VARIANT_FN(FnName, Return) \
    template <std::size_t pos = 0, typename VariantType, typename... Args> \
    Return FnName(VariantType n, Args&&... args) \
    { \
        using AlternativeType = std::variant_alternative_t<pos, VariantType>; \
     \
        if (std::holds_alternative<AlternativeType>(n)) \
        { \
            return FnName(std::get<pos>(n), std::forward<Args>(args)...); \
        } \
        if constexpr (pos + 1 < std::variant_size_v<VariantType>) \
        { \
            return FnName<pos + 1, VariantType>(n, std::forward<Args>(args)...); \
        } \
    } \

#define CUSTOM_VARIANT_FN(FnName, Return) \
    template <std::size_t pos = 0, typename CustomVariantType, typename... Args> \
    Return FnName(CustomVariantType& cn, Args&&... args) \
    { \
        using VariantType = typename CustomVariantType::_variant_type; \
        using AlternativeType = std::variant_alternative_t<pos, VariantType>; \
        auto& n = cn._variant; \
        if (std::holds_alternative<AlternativeType>(n)) \
        { \
            return FnName(std::get<pos>(n), std::forward<Args>(args)...); \
        } \
        if constexpr (pos + 1 < std::variant_size_v<VariantType>) \
        { \
            return FnName<pos + 1, CustomVariantType>(cn, std::forward<Args>(args)...); \
        } \
    } \

}