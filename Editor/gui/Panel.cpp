//
// Created by Stefan Annell on 2021-05-15.
//


#include "Panel.h"
#include "Camera.h"
#include "Core.h"
#include "Engine.h"
#include <BaseComponents.h>
#include <Factory.h>
#include <Lightsource.h>
#include <future>

namespace gui {

    void ShowSimpleOverlay(const std::string& text, bool *p_open) {
        const float PAD = 10.0f;
        static int corner = 1;
        ImGuiIO &io = ImGui::GetIO();
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
        if (corner != -1) {
            const ImGuiViewport *viewport = ImGui::GetMainViewport();
            ImVec2 work_pos = viewport->WorkPos;// Use work area to avoid menu-bar/task-bar, if any!
            ImVec2 work_size = viewport->WorkSize;
            ImVec2 window_pos, window_pos_pivot;
            window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
            window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
            window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
            window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
            window_flags |= ImGuiWindowFlags_NoMove;
        }
        ImGui::SetNextWindowBgAlpha(0.35f);// Transparent background
        if (ImGui::Begin("Example: Simple overlay", p_open, window_flags)) {
            ImGui::Text("%s", text.c_str());
            if (ImGui::BeginPopupContextWindow()) {
                if (ImGui::MenuItem("Custom", nullptr, corner == -1)) corner = -1;
                if (ImGui::MenuItem("Top-left", nullptr, corner == 0)) corner = 0;
                if (ImGui::MenuItem("Top-right", nullptr, corner == 1)) corner = 1;
                if (ImGui::MenuItem("Bottom-left", nullptr, corner == 2)) corner = 2;
                if (ImGui::MenuItem("Bottom-right", nullptr, corner == 3)) corner = 3;
                if (p_open && ImGui::MenuItem("Close")) *p_open = false;
                ImGui::EndPopup();
            }
        }
        ImGui::End();
    }

    void ShowEntityColorController(const voxie::Entity &entity) {
        ImGui::Separator();
        auto color = voxie::helper::GetComponent<voxie::Color>(entity);
        float updatedColor[3] = {color->color[0], color->color[1], color->color[2]};
        ImGui::ColorPicker3("Color", updatedColor);
        color->SetColor(updatedColor[0], updatedColor[1], updatedColor[2]);
    }

    void ShowEntityAttenuationController(const voxie::Entity &entity) {
        ImGui::Separator();
        auto attenuation = voxie::helper::GetComponent<voxie::Attenuation>(entity);
        ImGui::InputFloat("Quadratic", &attenuation->quadratic);
        ImGui::InputFloat("Linear", &attenuation->linear);
        ImGui::InputFloat("Constant", &attenuation->constant);
    }

    void ShowEntityPositionController(const voxie::Entity &entity) {
        ImGui::Separator();
        auto pos = voxie::helper::GetComponent<voxie::Position>(entity);
        float translation[3] = {pos->pos.x, pos->pos.y, pos->pos.z};
        float rotation[3] = {pos->rotation.x, pos->rotation.y, pos->rotation.z};
        float scale[3] = {pos->scale.x, pos->scale.y, pos->scale.z};
        ImGui::InputFloat3("Position", translation);
        ImGui::InputFloat3("Scale", scale);
        ImGui::InputFloat3("Rotation", rotation);
        pos->SetRotation({rotation[0], rotation[1], rotation[2]});
        pos->SetScale({scale[0], scale[1], scale[2]});
        pos->SetPosition({translation[0], translation[1], translation[2]});
        pos->UpdateModel();
    }

    void ShowEntityPosition2DController(const voxie::Entity &entity) {
        ImGui::Separator();
        auto pos = voxie::helper::GetComponent<voxie::Position2D>(entity);
        float translation[2] = {pos->pos.x, pos->pos.y};
        float rotation = pos->rotation;
        float scale[2] = {pos->scale.x, pos->scale.y};
        ImGui::InputFloat2("Position", translation);
        ImGui::InputFloat2("Scale", scale);
        ImGui::InputFloat("Rotation", &rotation);
        pos->SetRotation(rotation);
        pos->SetScale({scale[0], scale[1]});
        pos->SetPosition({translation[0], translation[1]});
        pos->UpdateModel();
    }

    void ShowEntityNameController(const voxie::Entity &entity) {
        ImGui::Separator();
        auto name = voxie::helper::GetComponent<voxie::Name>(entity);
        char *buf = (char *) name->name.c_str();
        auto callback = [](ImGuiInputTextCallbackData *data) -> int {
            auto *entity = (voxie::Entity *) data->UserData;
            auto name = voxie::helper::GetComponent<std::string>(*entity);
            name->resize(data->BufSize);
            *name = std::string(data->Buf);
            return 1;
        };
        ImGui::InputText("name", buf, 20, 0, callback, (void *) &entity);
    }

    void ShowEntityDirectionController(const voxie::Entity &entity) {
        ImGui::Separator();
        auto direction = voxie::helper::GetComponent<voxie::Direction>(entity);
        ImGui::InputFloat("Yaw", &direction->yaw);
        ImGui::InputFloat("Pitch", &direction->pitch);
    }

    void ShowCameraSelectorController(const voxie::Entity &entity) {
        if (ImGui::Button("Set as camera"))
            voxie::Engine::GetEngine().SetCamera(entity);
    }

    void ShowGuizmo(const voxie::Entity &entity) {
        ImGui::Separator();
        static auto mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
        static auto mCurrentGizmoMode = ImGuizmo::WORLD;
        if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
            mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
            mCurrentGizmoOperation = ImGuizmo::ROTATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
            mCurrentGizmoOperation = ImGuizmo::SCALE;

        if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
            mCurrentGizmoMode = ImGuizmo::LOCAL;
        ImGui::SameLine();
        if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
            mCurrentGizmoMode = ImGuizmo::WORLD;

        ImGuizmo::SetDrawlist(ImGui::GetBackgroundDrawList(ImGui::GetMainViewport()));
        ImGuizmo::SetRect(0, 0, voxie::Engine::GetEngine().GetWindow()->GetWidth(), voxie::Engine::GetEngine().GetWindow()->GetHeight());

        auto camera = voxie::Engine::GetEngine().GetCamera();
        auto pos = voxie::helper::GetComponent<voxie::Position>(entity);
        auto mModel = pos->model;
        const auto view = camera->GetViewMatrix();
        const auto proj = camera->GetProjectionMatrix();

        if (ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(proj), mCurrentGizmoOperation, mCurrentGizmoMode, glm::value_ptr(mModel), nullptr, nullptr, nullptr, nullptr)) {
            pos->SetModel(mModel);
        }
    }

    void AddNewComponent() {
        if (ImGui::Button("Add"))
            ImGui::OpenPopup("add_popup");
        if (ImGui::BeginPopup("add_popup")) {
            auto lights = voxie::GetLights();
            if (ImGui::BeginMenu("Lights")) {
                for (auto &light : lights) {
                    if (ImGui::Selectable(light.name.c_str())) {
                        auto obj = voxie::MakeLight(light);
                        auto entity = obj->GetEntity();
                        voxie::helper::AddComponent(entity, std::move(obj));
                    }
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Models")) {
                auto models = voxie::GetModels();
                for (auto &model : models) {
                    if (ImGui::Selectable(model.name.c_str())) {
                        auto obj = voxie::MakeModel(model);
                        auto entity = obj->GetEntity();
                        voxie::helper::AddComponent(entity, std::move(obj));
                    }
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Sprites")) {
                for (auto &sprite : voxie::GetSprites()) {
                    if (ImGui::Selectable(sprite.name.c_str())) {
                        auto obj = voxie::MakeSprite(sprite);
                        auto entity = obj->GetEntity();
                        voxie::helper::AddComponent(entity, std::move(obj));
                    }
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Camera")) {
                if (ImGui::Selectable("Camera")) {
                    auto camera = voxie::MakeCamera({"Camera"});
                    auto entity = camera->GetEntity();
                    voxie::helper::AddComponent(entity, std::move(camera));
                }
                ImGui::EndMenu();
            }
            ImGui::EndPopup();
        }
    }

    void RemoveComponent(const voxie::Entity &entity) {
        if (ImGui::Button("Remove")) {
            voxie::Engine::GetEngine().GetScene().RemoveEntity(entity);
        }
    }

    void ShowTopMenu() {
        if (ImGui::MenuItem("New")) {
            voxie::Engine::GetEngine().GetScene().ClearScene();
        }
        if (ImGui::BeginMenu("Open")) {
            for (auto &scene : voxie::GetScenes()) {
                if (ImGui::Selectable(scene.name.c_str())) {
                    voxie::Engine::GetEngine().GetScene().Load(scene.name);
                }
            }
            ImGui::EndMenu();
        }
        if (ImGui::MenuItem("Save", "Ctrl+S")) {
            voxie::Engine::GetEngine().GetScene().Save();
        }
        if (ImGui::MenuItem("Save As..")) {
            ImGui::OpenPopup("Save as");
        }
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        if (ImGui::BeginPopupModal("Save as", NULL, ImGuiWindowFlags_MenuBar))
        {
            ImGui::Text("Save scene as:");
            ImGui::Separator();

            if (ImGui::Button("OK", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if (ImGui::MenuItem("Quit", "Alt+F4")) {
            glfwSetWindowShouldClose(voxie::Engine::GetEngine().GetWindow()->GetWindow(), 1);
        }
    }

    void ShowMainMenuBar() {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ShowTopMenu();
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }

    auto ShowEntityList() {
        static int selected = 0;
        ImGui::Begin("Scene entities");
        std::vector<const char *> items;
        auto &entities = voxie::Engine::GetEngine().GetScene().GetEntities();

        for (auto &entity : entities) {
            auto name = voxie::helper::GetComponent<voxie::Name>(entity);
            items.push_back(name->name.c_str());
        }
        ImGui::ListBox("", &selected, &items[0], entities.size());

        return selected >= entities.size() ? entities.back() : entities.at(selected);
    }

    void ShowSceneOverview() {
        ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);
        ImGuizmo::BeginFrame();
        ShowMainMenuBar();
        auto entity = ShowEntityList();

        AddNewComponent();
        ImGui::SameLine();
        RemoveComponent(entity);

        if (voxie::helper::HasComponent<voxie::Name>(entity)) {
            ShowEntityNameController(entity);
        }

        if (voxie::helper::HasComponent<voxie::Position>(entity)) {
            ShowEntityPositionController(entity);
            ShowGuizmo(entity);
        }

        if (voxie::helper::HasComponent<voxie::Position2D>(entity)) {
            ShowEntityPosition2DController(entity);
        }

        if (voxie::helper::HasComponent<voxie::Direction>(entity)) {
            ShowEntityDirectionController(entity);
        }

        if (voxie::helper::HasComponent<voxie::Color>(entity)) {
            ShowEntityColorController(entity);
        }

        if (voxie::helper::HasComponent<voxie::Attenuation>(entity)) {
            ShowEntityAttenuationController(entity);
        }

        if (voxie::helper::HasComponent<voxie::Camera>(entity)) {
            ShowCameraSelectorController(entity);
        }

        ImGui::End();
    }


}// namespace gui
