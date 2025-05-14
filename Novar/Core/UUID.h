#pragma once

#include <functional>

namespace NV
{
    class UUID
    {
    public:
        UUID();
        UUID(uint64_t uuid);
        UUID(const UUID& other) = default;
        UUID& operator=(const UUID& other) = default;

        operator uint64_t() const { return m_UUID; }
    private:
            uint64_t m_UUID;
    };
}

namespace std
{
    template<>
    struct hash<NV::UUID>
    {
        size_t operator()(const NV::UUID& uuid) const
        {
            return (size_t)uuid;
        }
    };
}