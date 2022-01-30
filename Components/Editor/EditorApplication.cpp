#include <memory>

#include "Core.h"
#include "EditorGameMode.h"


int main() {
    auto &engine = voxie::Engine::GetEngine();
    engine.Init();
    engine.LoadGameMode(std::make_unique<voxie::EditorGameMode>());
    engine.GetWindow()->SetTitle("Voxie Editor");
    engine.StartLoop();
    return 0;
}
