#ifndef BENGINE_CORE_MEMORY_HPP
#define BENGINE_CORE_MEMORY_HPP

#include <memory>

namespace BE::Core::Memory
{

// NOTE: Stoled from Hazel Engine (under Apache License 2.0)
// <https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Core/Base.h>

template <typename T> using Scope = std::unique_ptr<T>;
template <typename T, typename... Args> constexpr Scope<T> MakeScope(Args &&...args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T> using Ref = std::shared_ptr<T>;
template <typename T, typename... Args> constexpr Ref<T> MakeRef(Args &&...args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

} // namespace BE::Core::Memory

#endif // BENGINE_CORE_MEMORY_HPP