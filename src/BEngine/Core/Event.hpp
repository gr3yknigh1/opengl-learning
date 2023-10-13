#ifndef BENGINE_CORE_EVENT_HPP
#define BENGINE_CORE_EVENT_HPP

#include <algorithm>
#include <functional>
#include <vector>

#include "Memory.hpp"

namespace BE::Core
{

template <typename... Args> using Proc = std::function<void(Args...)>;

template <typename... Args> class Event
{
public:
    using Callback = Proc<Args...>;

    void Subscribe(const Memory::Ref<Callback> &callback)
    {
        m_Callbacks.push_back(callback);
    }

    void Invoke(Args... payload) const
    {
        const auto callCallback = [=](Memory::Ref<Callback> callback) { (*(callback.get()))(payload...); };

        std::for_each(m_Callbacks.begin(), m_Callbacks.end(), callCallback);
    }

    void RemoveFirst(const Memory::Ref<Callback> &callbackToRemove)
    {
        const auto callbackSearchResult =
            std::find_if(m_Callbacks.cbegin(), m_Callbacks.cend(),
                         [=](const Memory::Ref<Callback> &callback) { return callback.get() == callbackToRemove.get(); });

        if (callbackSearchResult != m_Callbacks.cend())
        {
            m_Callbacks.erase(callbackSearchResult);
        }
    }

    void Clear(void)
    {
        m_Callbacks.erase(m_Callbacks.begin(), m_Callbacks.end());
    }

    constexpr const std::vector<Memory::Ref<Callback>> &GetSubscribers(void) const
    {
        return m_Callbacks;
    }

private:
    std::vector<Memory::Ref<Callback>> m_Callbacks;
};

} // namespace BE::Core

#endif // BENGINE_CORE_EVENT_HPP