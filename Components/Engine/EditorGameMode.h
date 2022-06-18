//
// Created by Stefan Annell on 2022-01-30.
//

#pragma once

#include "EntityComponentSystem.h"
#include "GameMode.h"

namespace voxie {


    class EditorGameMode : public GameMode {
    public:
        EditorGameMode();

        void OnTick(float deltaTime) override;

        void Start() override;

        Handle GetSelection() const;
        void SetSelection(const Handle &);

    private:
        Handle selection = NullEntity;
    };

}// namespace voxie