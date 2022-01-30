//
// Created by Stefan Annell on 2022-01-30.
//

#pragma once

#include "GameMode.h"
namespace voxie {


class EditorGameMode : public GameMode {
public:
    EditorGameMode();

    void Initialize() override;
    void OnTick(float deltaTime) override;
private:
};

}