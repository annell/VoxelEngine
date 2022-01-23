//
// Created by Stefan Annell on 2022-01-23.
//

#include "Text.h"
#include "Core.h"

namespace voxie {

    Text::Text(const Handle & handle, std::shared_ptr<Name> name, std::shared_ptr<Position2D> position, std::shared_ptr<Color> color, std::shared_ptr<VisibleText> visibleText)
        : handle(handle) {
        helper::AddComponent(handle, std::move(name));
        helper::AddComponent(handle, std::move(position));
        helper::AddComponent(handle, std::move(color));
        helper::AddComponent(handle, std::move(visibleText));
    }

    Text::~Text() {
        helper::RemoveComponent<Name>(handle);
        helper::RemoveComponent<Position2D>(handle);
        helper::RemoveComponent<Color>(handle);
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
        GetColor()->decode(node["color"]);
        GetVisibleText()->decode(node["text"]);
        return true;
    }

    std::shared_ptr<Position2D> Text::GetPosition2D() const {
        return voxie::helper::GetComponent<Position2D>(handle);
    }

    std::shared_ptr<VisibleText> Text::GetVisibleText() const {
        return voxie::helper::GetComponent<VisibleText>(handle);
    }

    const Handle &Text::GetHandle() const {
        return handle;
    }
    std::shared_ptr<Color> Text::GetColor() const {
        return voxie::helper::GetComponent<Color>(handle);
    }
}