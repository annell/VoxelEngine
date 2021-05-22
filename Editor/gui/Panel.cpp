//
// Created by Stefan Annell on 2021-05-15.
//

#include <BaseComponents.h>
#include <Lightsource.h>
#include "Panel.h"
#include "Engine.h"

namespace gui {

void ShowEntityColorController(const voxie::Entity& entity) {
    auto color = voxie::helper::GetComponent<voxie::Color>(entity);
    float updatedColor[3] = {color->color[0], color->color[1], color->color[2]};
    ImGui::ColorPicker3("Color", updatedColor);
    color->SetColor(updatedColor[0], updatedColor[1], updatedColor[2]);
}

void ShowEntityAtteunationController(const voxie::Entity& entity) {
    auto atteunation = voxie::helper::GetComponent<voxie::Atteunation>(entity);
    ImGui::InputFloat("Quadratic", &atteunation->quadratic);
    ImGui::InputFloat("Linear", &atteunation->linear);
    ImGui::InputFloat("Constant", &atteunation->constant);
}

void ShowEntityPositionController(const voxie::Entity& entity) {
    auto pos = voxie::helper::GetComponent<voxie::Position>(entity);
    float translation[3] = {pos->pos.x, pos->pos.y, pos->pos.z};
    ImGui::InputFloat3("Position", translation);
    pos->SetPosition(translation[0], translation[1], translation[2]);
}

void ShowEntityDirectionController(const voxie::Entity& entity) {
    auto direction = voxie::helper::GetComponent<voxie::Direction>(entity);
    ImGui::InputFloat("Yaw", &direction->yaw);
    ImGui::InputFloat("Pitch", &direction->pitch);
}

void ShowSceneOverview() {
    static int selected = 0;
    ImGui::Begin("Controllers");
    std::vector<const char*> items;
    auto& entities = voxie::Engine::GetEngine().GetScene().GetEntities();
    for (auto& entity : entities) {
        items.push_back(entity->GetName().c_str());
    }
    ImGui::ListBox("", &selected, &items[0], entities.size());

    auto& entity = *entities.at(selected);
    if (voxie::helper::HasComponent<voxie::Position>(entity)) {
        ShowEntityPositionController(entity);
    }

    if (voxie::helper::HasComponent<voxie::Direction>(entity)) {
        ShowEntityDirectionController(entity);
    }

    if (voxie::helper::HasComponent<voxie::Color>(entity)) {
        ShowEntityColorController(entity);
    }

    if (voxie::helper::HasComponent<voxie::Atteunation>(entity)) {
        ShowEntityAtteunationController(entity);
    }

    ImGui::End();
}

void ShowSimpleOverlay(std::string text, bool* p_open)
{
    const float PAD = 10.0f;
    static int corner = 1;
    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    if (corner != -1)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos, window_pos_pivot;
        window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
        window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
        window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
        window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        window_flags |= ImGuiWindowFlags_NoMove;
    }
    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    if (ImGui::Begin("Example: Simple overlay", p_open, window_flags))
    {
        ImGui::Text(text.c_str());
        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("Custom",       NULL, corner == -1)) corner = -1;
            if (ImGui::MenuItem("Top-left",     NULL, corner == 0)) corner = 0;
            if (ImGui::MenuItem("Top-right",    NULL, corner == 1)) corner = 1;
            if (ImGui::MenuItem("Bottom-left",  NULL, corner == 2)) corner = 2;
            if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
            if (p_open && ImGui::MenuItem("Close")) *p_open = false;
            ImGui::EndPopup();
        }
    }
    ImGui::End();
}

}
