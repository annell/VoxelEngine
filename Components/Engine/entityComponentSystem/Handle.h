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

}// namespace voxie

namespace std {

    template<>
    struct hash<voxie::Handle> {
        std::size_t operator()(const voxie::Handle &handle) const {
            return std::hash<voxie::Handle::Identity>()(handle.GetId());
        }
    };

}// namespace std
