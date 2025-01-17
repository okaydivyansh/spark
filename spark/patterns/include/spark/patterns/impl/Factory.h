#pragma once

#include "spark/patterns/details/Creators.h"

#include "spark/base/Exception.h"

namespace spark::patterns
{
    template <typename Key, typename BaseType, typename... Args>
    template <typename TypeToRegister>
    void Factory<Key, BaseType, Args...>::registerType(const Key& key)
    {
        if (m_creators.contains(key))
            SPARK_BASE_THROW_EXCEPTION(spark::base::DuplicatedParameterException, "Type is already registered into the factory.");
        m_creators[key] = std::make_unique<details::DerivedCreator<BaseType, TypeToRegister, Args...>>();
    }

    template <typename Key, typename BaseType, typename ... Args>
    typename Factory<Key, BaseType, Args...>::BasePtr Factory<Key, BaseType, Args...>::create(const Key& key, const Args&... args) const
    {
        if (!m_creators.contains(key))
            SPARK_BASE_THROW_EXCEPTION(spark::base::CouldNotFindParameterException, "Type is not registered into the factory.");
        return m_creators.at(key)->create(args...);
    }

    template <typename Key, typename BaseType, typename... Args>
    typename Factory<Key, BaseType, Args...>::BasePtr Factory<Key, BaseType, Args...>::createOrFail(const Key& key, const Args&... args) const noexcept
    {
        if (!m_creators.contains(key))
            return nullptr;
        return m_creators.at(key)->create(args...);
    }

    template <typename Key, typename BaseType, typename... Args>
    std::vector<Key> Factory<Key, BaseType, Args...>::getRegisteredTypes() const noexcept
    {
        std::vector<Key> keys;
        keys.reserve(m_creators.size());
        for (const auto& [key, ptr] : m_creators)
            keys.push_back(key);
        return keys;
    }
}
