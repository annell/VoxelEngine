//
// Created by Stefan Annell on 2022-01-23.
//

#include "Text.h"
#include "Core.h"

namespace voxie {

    Text::Text(const Handle &handle, std::shared_ptr<Name> name, std::shared_ptr<Position2D> position, std::shared_ptr<Color> color, std::shared_ptr<VisibleText> visibleText)
        : NodeWrapper(handle) {
        COMPONENT_REGISTER(Position2D, position);
        COMPONENT_REGISTER(Name, name);
        COMPONENT_REGISTER(VisibleText, visibleText);
        COMPONENT_REGISTER(Color, color);
    }

    Text::~Text() {
    }

    void Text::encode(YAML::Node &node) const {
        node["type"] = "Text";
        node["id"] = GetHandle().GetId();
        auto name = helper::GetComponent<Name>(handle).get();
        node["name"] = name->name;
        node["position"] = *helper::GetComponent<Position2D>(handle).get();
        node["color"] = *helper::GetComponent<Color>(handle).get();
        node["text"] = *helper::GetComponent<VisibleText>(handle).get();
    }

    bool Text::decode(const YAML::Node &node) {
        GetPosition2D()->decode(node["position"]);
        DecodeComponent(node["color"], GetColor());
        GetVisibleText()->decode(node["text"]);
        return true;
    }
}// namespace voxie