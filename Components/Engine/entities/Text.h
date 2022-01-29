//
// Created by Stefan Annell on 2022-01-23.
//

#pragma once

#include <memory>
#include "Node.h"

namespace voxie {

    class Name;
    class Position2D;
    class Color;
    class VisibleText;

class Text : public NodeWrapper {
public:
    Text(const Handle &, std::shared_ptr<Name>, std::shared_ptr<Position2D>, std::shared_ptr<Color>, std::shared_ptr<VisibleText>);
    ~Text() override;

    const Handle &GetHandle() const override;
    void encode(YAML::Node &) const;
    bool decode(const YAML::Node &);

    std::shared_ptr<Position2D> GetPosition2D() const;
    std::shared_ptr<Color> GetColor() const;
    std::shared_ptr<VisibleText> GetVisibleText() const;
    std::shared_ptr<Name> GetName() const;
private:
    Handle handle;
};
}
