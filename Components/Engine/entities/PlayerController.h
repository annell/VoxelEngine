#pragma once
#include "Node.h"
#include <memory>

namespace voxie {

    class PlayerController : public NodeWrapper {
    public:
        PlayerController(const Handle &handle, std::shared_ptr<class Name> name, std::shared_ptr<class Position> position);

        void OnTick(float delta);
        void SetCamera(const Handle &);
        std::shared_ptr<class Camera> GetCamera();

    private:
        int OnTickHandle = 0;
        Handle CurrentView;
    };

}// namespace voxie