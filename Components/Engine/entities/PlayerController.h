#pragma once
#include "Node.h"
#include <memory>

namespace voxie {

    class RigidBody;
    struct Verlet;

    class PlayerController : public NodeWrapper {
    public:
        PlayerController(const Handle &handle, std::shared_ptr<class Name> name, std::shared_ptr<class Position> position);

        void BeginPlay() override;
        void StopPlay() override;

        void OnTick(float delta);
        void SetCamera(const Handle &);
        std::shared_ptr<class Camera> GetCamera();

        COMPONENT_ADD_FUNCTION(Position);
        COMPONENT_ADD_FUNCTION(RigidBody);
        COMPONENT_ADD_FUNCTION(Verlet);

        void encode(YAML::Node &) const;
        bool decode(const YAML::Node &);

    private:
        void Movement(const glm::vec3 &direction);
        int OnTickHandle = 0;
        Handle CurrentView;

        glm::vec3 MovementVelocity;

        bool jumped = false;
        float jumpHeight = 20;
    };

}// namespace voxie