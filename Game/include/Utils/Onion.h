#pragma once
#include <type_traits>
#include <variant>

namespace utils
{

#define VARIANT_FN(FnName, Return) \
    template <std::size_t pos = 0, typename VariantType, typename... Args> \
    Return FnName(VariantType&& n, Args&&... args) \
    { \
        using UnqualifiedVariantType = std::remove_reference_t<std::remove_cv_t<VariantType>>; \
        using AlternativeType = std::variant_alternative_t<pos, UnqualifiedVariantType>; \
      \
        if (std::holds_alternative<AlternativeType>(n)) \
        { \
            return FnName(std::get<pos, UnqualifiedVariantType>(n), std::forward<Args>(args)...); \
        } \
        if constexpr (pos + 1 < std::variant_size_v<UnqualifiedVariantType>) \
        { \
            return FnName<pos + 1, VariantType>(n, std::forward<Args>(args)...); \
        } \
    } \

}