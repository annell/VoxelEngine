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

    void ShowSimpleOverlay(const std::string &text, bool *p_open) {
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
        ImGui::SliderFloat("Quadratic", &attenuation->quadratic, 0.0f, 100.0f);
        ImGui::SliderFloat("Linear", &attenuation->linear, 0.0f, 100.0f);
        ImGui::SliderFloat("Constant", &attenuation->constant, 0.0f, 100.0f);
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

    void ShowSceneNameController() {
        ImGui::Separator();
        auto &name = voxie::Engine::GetEngine().GetScene().GetSceneName();
        char *buf = (char *) name.c_str();
        auto callback = [](ImGuiInputTextCallbackData *data) -> int {
            return 1;
        };
        ImGui::InputText("Scene Name", buf, 20, 0, callback, (void *) nullptr);
        voxie::Engine::GetEngine().GetScene().SetFilename(std::string(buf));
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
                        voxie::Engine::GetEngine().GetScene().AddNode(voxie::MakeLight(light), nullptr);
                    }
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Models")) {
                auto models = voxie::GetModels();
                for (auto &model : models) {
                    if (ImGui::Selectable(model.name.c_str())) {
                        voxie::Engine::GetEngine().GetScene().AddNode(voxie::MakeModel(model), nullptr);
                    }
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Sprites")) {
                for (auto &sprite : voxie::GetSprites()) {
                    if (ImGui::Selectable(sprite.name.c_str())) {
                        voxie::Engine::GetEngine().GetScene().AddNode(voxie::MakeSprite(sprite), nullptr);
                    }
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Camera")) {
                if (ImGui::Selectable("Camera")) {
                    voxie::Engine::GetEngine().GetScene().AddNode(voxie::MakeCamera({"Camera", voxie::Entity::MakeEntity()}), nullptr);
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Transform Node")) {
                if (ImGui::Selectable("Transform Node")) {
                    voxie::Engine::GetEngine().GetScene().AddNode(voxie::MakeTransformNode({"Transform Node", voxie::Entity::MakeEntity()}), nullptr);
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
            voxie::Engine::GetEngine().GetScene().AddNode(voxie::MakeTransformNode({"Transform Node", voxie::Entity::MakeEntity()}), nullptr);
            auto camera = voxie::Entity::MakeEntity();
            voxie::Engine::GetEngine().GetScene().AddNode(voxie::MakeCamera({"Editor Camera", camera}), nullptr);
            voxie::Engine::GetEngine().SetCamera(camera);
        }
        if (ImGui::BeginMenu("Open")) {
            for (auto &scene : voxie::GetScenes()) {
                if (ImGui::Selectable(scene.name.c_str())) {
                    voxie::Engine::GetEngine().GetScene().Load(scene.name);
                }
            }
            ImGui::EndMenu();
        }
        if (ImGui::MenuItem("Save")) {
            voxie::Engine::GetEngine().GetScene().Save();
        }
        if (ImGui::MenuItem("Quit")) {
            glfwSetWindowShouldClose(voxie::Engine::GetEngine().GetWindow()->GetWindow(), 1);
        }
    }

    void ShowMainMenuBar() {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                ShowTopMenu();
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }

    bool DisplayNode(voxie::Node * root, voxie::Entity& selected, int flags) {
        #define IMGUI_PAYLOAD_TYPE_SCENENODE "_SCENENODE"
        static voxie::Node * draggedNode = nullptr;
        auto rootNode = root->GetNode();
        auto name = voxie::helper::GetComponent<voxie::Name>(rootNode);
        bool isSelected = rootNode == selected;
        if (isSelected) {
            flags |= ImGuiTreeNodeFlags_Selected;
        }
        bool open = ImGui::TreeNodeEx(name->name.c_str(), flags);
        if (ImGui::IsItemClicked()) {
            selected = rootNode;
        }
        if (ImGui::BeginDragDropSource()) {
            ImGui::SetDragDropPayload(IMGUI_PAYLOAD_TYPE_SCENENODE, NULL, 0);
            draggedNode = root;
            ImGui::Text("%s", name->name.c_str());
            ImGui::EndDragDropSource();
        }

        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_SCENENODE)) {
                draggedNode->MoveTo(root);
                draggedNode = nullptr;
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::TableNextColumn();
        return open;
    }

    void DisplayTreeView(voxie::Node * root, voxie::Entity& selected) {
        if (!root) {
            return;
        }
        bool isFolder = root->GetNumChildren() > 0;
        if (isFolder) {
            if (DisplayNode(root, selected, ImGuiTreeNodeFlags_SpanFullWidth)) {
                for (const auto& node : root->GetChildNodes()) {
                    DisplayTreeView(node.get(), selected);
                }
                ImGui::TreePop();
            }
        } else {
            DisplayNode(root, selected, ImGuiTreeNodeFlags_Leaf  | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);
            for (const auto& node : root->GetChildNodes()) {
                DisplayTreeView(node.get(), selected);
            }
        }
    }

    void ShowTreeView(voxie::Entity& selected) {
        static ImGuiTableFlags flags = ImGuiTableFlags_Resizable
                                       | ImGuiTableFlags_NoBordersInBody;

        if (ImGui::BeginTable("Entities", 1, flags)) {
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
            ImGui::TableNextRow();
            ImGui::TableNextColumn();

            DisplayTreeView(voxie::Engine::GetEngine().GetScene().GetRoot(), selected);
            ImGui::EndTable();
        }
    }

    auto ShowEntityList() {
        ImGui::Separator();
        std::vector<const char *> items;
        auto entities = voxie::Engine::GetEngine().GetScene().GetEntities();
        static auto selected = entities.front();

        ShowTreeView(selected);

        return selected;
    }

    void ShowSceneOverview() {
        ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);
        ImGuizmo::BeginFrame();
        ShowMainMenuBar();
        ImGui::Begin("Scene");
        ShowSceneNameController();
        auto entity = ShowEntityList();
        voxie::Engine::GetEngine().GetCamera()->SetSelection(entity);

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
