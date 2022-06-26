#pragma once
#include "Node.h"
#include <memory>

namespace voxie {

    class RigidBody;

    class PlayerController : public NodeWrapper {
    public:
        PlayerController(const Handle &handle, std::shared_ptr<class Name> name, std::shared_ptr<class Position> position);

        void OnTick(float delta);
        void SetCamera(const Handle &);
        std::shared_ptr<class Camera> GetCamera();

        COMPONENT_ADD_FUNCTION(Position);
        COMPONENT_ADD_FUNCTION(RigidBody);

        void encode(YAML::Node &) const;
        bool decode(const YAML::Node &);

    private:
        int OnTickHandle = 0;
        Handle CurrentView;
    };

}// namespace voxie