//
// Created by Stefan Annell on 2022-01-23.
//

#pragma once
#include <cstdint>
#include <functional>

namespace voxie {


class Handle {
public:
    using Identity = u_int64_t;
    explicit Handle(Identity);

    static Handle MakeEntity();

    Handle::Identity GetId() const;

    bool operator<(const Handle &rhs) const;

    bool operator==(const Handle &rhs) const;


private:
    auto as_tie() const;

    Handle::Identity id;
};

}

namespace std {

    template <>
    struct hash<voxie::Handle>
    {
        std::size_t operator()(const voxie::Handle& handle) const
        {
            // Compute individual hash values for first,
            // second and third and combine them using XOR
            // and bit shifting:

            return std::hash<voxie::Handle::Identity>()(handle.GetId());
        }
    };

}
