//
// Created by Stefan Annell on 2022-01-23.
//

#pragma once
#include <cstdint>

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
    [[nodiscard]] auto as_tie() const;

    Handle::Identity id;
};

}
