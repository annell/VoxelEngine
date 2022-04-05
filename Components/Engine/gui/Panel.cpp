//

#include "Panel.h"
#include "Core.h"
#include "CubeEntity.h"
#include "EditorGameMode.h"
#include <GLFW/glfw3.h>
#include <GameMode.h>
namespace internal {

    struct InputTextCallback_UserData {
        std::string *Str;
        ImGuiInputTextCallback ChainCallback;
        void *ChainCallbackUserData;
    };

    static int InputTextCallback(ImGuiInputTextCallbackData *data) {
        InputTextCallback_UserData *user_data = (InputTextCallback_UserData *) data->UserData;
        if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
            // Resize string callback
            // If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
            std::string *str = user_data->Str;
            IM_ASSERT(data->Buf == str->c_str());
            str->resize(data->BufTextLen);
            data->Buf = (char *) str->c_str();
        } else if (user_data->ChainCallback) {
            // Forward to user callback, if any
            data->UserData = user_data->ChainCallbackUserData;
            return user_data->ChainCallback(data);
        }
        return 0;
    }

    bool InputText(const char *label, std::string *str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void *user_data = NULL) {
        IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
        flags |= ImGuiInputTextFlags_CallbackResize;

        InputTextCallback_UserData cb_user_data;
        cb_user_data.Str = str;
        cb_user_data.ChainCallback = callback;
        cb_user_data.ChainCallbackUserData = user_data;
        return ImGui::InputText(label, (char *) str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
    }

}// namespace internal

namespace gui {

    void ShowEntityColorController(const voxie::Handle &entity) {
        if (ImGui::CollapsingHeader("Color")) {
            auto color = voxie::helper::GetComponent<voxie::Color>(entity);
            float updatedColor[3] = {color->color[0], color->color[1], color->color[2]};
            ImGui::ColorEdit3("Color", updatedColor);
            color->SetColor(updatedColor[0], updatedColor[1], updatedColor[2]);
        }
    }

    void ShowEntityInvertedHullController(const voxie::Handle &entity) {
        if (ImGui::CollapsingHeader("Outline")) {
            auto invertedHull = voxie::helper::GetComponent<voxie::InvertedHull>(entity);
            ImGui::SliderFloat("Scale", &invertedHull->scale, 1, 1.5);
        }
    }

    void ShowEntityAttenuationController(const voxie::Handle &entity) {
        if (ImGui::CollapsingHeader("Attenuation")) {
            auto attenuation = voxie::helper::GetComponent<voxie::Attenuation>(entity);
            ImGui::SliderFloat("Quadratic", &attenuation->quadratic, 0.0f, 5.0f);
            ImGui::SliderFloat("Linear", &attenuation->linear, 0.0f, 5.0f);
            ImGui::SliderFloat("Constant", &attenuation->constant, 0.0f, 5.0f);
        }
    }

    void ShowEntityMaterialController(const voxie::Handle &entity) {
        if (ImGui::CollapsingHeader("Material")) {
            ImGui::Separator();
            auto material = voxie::helper::GetComponent<voxie::Material>(entity);


            ImGui::Separator();
            float ambient[3] = {material->ambient[0], material->ambient[1], material->ambient[2]};
            float diffuse[3] = {material->diffuse[0], material->diffuse[1], material->diffuse[2]};
            float specular[3] = {material->specular[0], material->specular[1], material->specular[2]};
            ImGui::ColorEdit3("Ambient", ambient);
            ImGui::ColorEdit3("Diffuse", diffuse);
            ImGui::ColorEdit3("Specular", specular);
            ImGui::SliderFloat("Shininess", &material->shininess, 0.0f, 100.0f);

            material->ambient = {ambient[0], ambient[1], ambient[2]};
            material->diffuse = {diffuse[0], diffuse[1], diffuse[2]};
            material->specular = {specular[0], specular[1], specular[2]};

            if (auto obj = voxie::helper::GetSceneNode<voxie::CubeEntity>(entity)) {
                obj->RefreshMaterial();
            }
        }
    }

    bool ShowGuizmo(const voxie::Handle &entity);

    void ShowEntityPositionController(const voxie::Handle &entity) {
        if (ImGui::CollapsingHeader("Position")) {
            auto pos = voxie::helper::GetComponent<voxie::Position>(entity);
            float translation[3] = {pos->pos.x, pos->pos.y, pos->pos.z};
            float rotation[3] = {pos->rotation.x, pos->rotation.y, pos->rotation.z};
            float scale[3] = {pos->scale.x, pos->scale.y, pos->scale.z};
            ImGui::DragFloat3("Position", translation);
            ImGui::DragFloat3("Scale", scale);
            ImGui::DragFloat3("Rotation", rotation);
            pos->SetRotation({rotation[0], rotation[1], rotation[2]});
            pos->SetScale({scale[0], scale[1], scale[2]});
            pos->SetPosition({translation[0], translation[1], translation[2]});

            if (ShowGuizmo(entity)) {
            }
            pos->SetModel(pos->model);
            if (auto rigidBody = voxie::helper::GetComponent<voxie::RigidBody>(entity)) {
                rigidBody->SetPosition(*pos);
            }
        }
    }

    void ShowEntityBodyController(const voxie::Handle &entity) {
        if (ImGui::CollapsingHeader("RigidBody")) {
            auto body = voxie::helper::GetComponent<voxie::RigidBody>(entity);
            auto bodyType = body->GetBodyType();

            if (ImGui::RadioButton("Kinematic", bodyType == voxie::BodyType::KINEMATIC)) {
                body->SetBodyType(voxie::BodyType::KINEMATIC);
            }

            if (ImGui::RadioButton("Static", bodyType == voxie::BodyType::STATIC)) {
                body->SetBodyType(voxie::BodyType::STATIC);
            }

            if (ImGui::RadioButton("Dynamic", bodyType == voxie::BodyType::DYNAMIC)) {
                body->SetBodyType(voxie::BodyType::DYNAMIC);
            }

            ImGui::Separator();

            float mass = body->GetMass();
            ImGui::DragFloat("Mass [kg]", &mass);
            body->SetMass(mass);

            bool gravity = body->GetGravity();
            ImGui::Checkbox("Gravity", &gravity);
            body->SetGravity(gravity);
        }
    }

    void ShowEntityPosition2DController(const voxie::Handle &entity) {
        if (ImGui::CollapsingHeader("Position2D")) {
            auto pos = voxie::helper::GetComponent<voxie::Position2D>(entity);
            float translation[2] = {pos->pos.x, pos->pos.y};
            float rotation = pos->rotation;
            float scale[2] = {pos->scale.x, pos->scale.y};
            ImGui::SliderFloat2("Position", translation, -1000, 1000);
            ImGui::SliderFloat2("Scale", scale, 0, 10);
            ImGui::SliderFloat("Rotation", &rotation, 0, 90);
            pos->SetRotation(rotation);
            pos->SetScale({scale[0], scale[1]});
            pos->SetPosition({translation[0], translation[1]});
            pos->UpdateModel();
        }
    }

    void ShowEntityNameController(const voxie::Handle &entity) {
        if (ImGui::CollapsingHeader("Name")) {
            auto name = voxie::helper::GetComponent<voxie::Name>(entity);
            internal::InputText("name", &name->name);
        }
    }

    void ShowEntityVisibleTextController(const voxie::Handle &entity) {
        if (ImGui::CollapsingHeader("Visible Text")) {
            ImGui::Separator();
            auto name = voxie::helper::GetComponent<voxie::VisibleText>(entity);
            internal::InputText("text", &name->text);
        }
    }

    void ShowSceneNameController() {
        auto &name = voxie::Engine::GetEngine().GetScene().GetSceneName();
        char *buf = (char *) name.c_str();
        auto callback = [](ImGuiInputTextCallbackData *data) -> int {
            return 1;
        };
        ImGui::InputText("Scene Name", buf, 20, 0, callback, (void *) nullptr);
        voxie::Engine::GetEngine().GetScene().SetFilename(std::string(buf));
    }

    void ShowEntityDirectionController(const voxie::Handle &entity) {
        if (ImGui::CollapsingHeader("Direction")) {
            ImGui::Separator();
            auto direction = voxie::helper::GetComponent<voxie::Direction>(entity);
            ImGui::DragFloat("Yaw", &direction->yaw);
            ImGui::DragFloat("Pitch", &direction->pitch);
        }
    }

    void ShowCameraSelectorController(const voxie::Handle &entity) {
        if (ImGui::CollapsingHeader("Camera")) {
            if (ImGui::Button("Set as camera"))
                voxie::Engine::GetEngine().SetCamera(entity);
        }
    }

    bool ShowGuizmo(const voxie::Handle &entity) {
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

        if (auto camera = voxie::Engine::GetEngine().GetCamera()) {

            auto pos = voxie::helper::GetComponent<voxie::Position>(entity);
            const auto view = camera->GetViewMatrix();
            const auto proj = camera->GetProjectionMatrix();

            if (ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(proj), mCurrentGizmoOperation, mCurrentGizmoMode, glm::value_ptr(pos->model), nullptr, nullptr, nullptr, nullptr)) {
                return true;
            }
        }
        return false;
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

            if (ImGui::BeginMenu("Base primitives")) {
                for (const auto &primitive : voxie::GetBasePrimitives()) {
                    if (primitive == voxie::BasePrimitives::Cube) {
                        if (ImGui::Selectable("Cube")) {
                            voxie::Engine::GetEngine().GetScene().AddNode(voxie::MakePrimitive({"Cube", voxie::BasePrimitives::Cube}), nullptr);
                        }
                    }
                }
                if (ImGui::Selectable("Text")) {
                    voxie::Engine::GetEngine().GetScene().AddNode(voxie::MakeText({"Text", "Text"}), nullptr);
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
                    voxie::Engine::GetEngine().GetScene().AddNode(voxie::MakeCamera({"Camera", voxie::Handle::MakeEntity()}), nullptr);
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Transform Node")) {
                if (ImGui::Selectable("Transform Node")) {
                    voxie::Engine::GetEngine().GetScene().AddNode(voxie::MakeTransformNode({"Transform Node", voxie::Handle::MakeEntity()}), nullptr);
                }
                ImGui::EndMenu();
            }
            ImGui::EndPopup();
        }
    }

    void RemoveComponent(const voxie::Handle &entity) {
        if (ImGui::Button("Remove")) {
            voxie::Engine::GetEngine().GetScene().RemoveEntity(entity);
        }
    }

    void ShowTopMenu() {
        if (ImGui::MenuItem("New")) {
            voxie::Engine::GetEngine().GetScene().ClearScene();
            voxie::Engine::GetEngine().GetScene().AddNode(voxie::MakeTransformNode({"Transform Node", voxie::Handle::MakeEntity()}), nullptr);
            auto camera = voxie::Handle::MakeEntity();
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

            auto *gameMode = voxie::Engine::GetEngine().GetGameMode();

            bool started = gameMode->IsStarted();
            float spacing = 10;
            static bool pressedPlay = false;
            if (started) {
                if (ImGui::SmallButton("Pause")) {
                    gameMode->Stop();
                }
                ImGui::SameLine(0, spacing);

            } else {
                if (!pressedPlay) {
                    if (ImGui::SmallButton("Play")) {
                        gameMode->Start();
                        pressedPlay = true;
                    }
                    ImGui::SameLine(0, spacing);
                } else {
                    if (ImGui::SmallButton("Resume")) {
                        gameMode->Resume();
                    }
                    ImGui::SameLine(0, spacing);
                }
            }
            if (pressedPlay) {
                if (ImGui::SmallButton("Stop")) {
                    gameMode->Reset();
                    pressedPlay = false;
                }
            }


            ImGui::EndMainMenuBar();
        }
    }

    bool DisplayNode(voxie::Node *root, voxie::Handle &selected, int flags) {
#define IMGUI_PAYLOAD_TYPE_SCENENODE "_SCENENODE"
        static voxie::Node *draggedNode = nullptr;
        auto rootNode = root->GetHandle();
        auto name = voxie::helper::GetComponent<voxie::Name>(rootNode);
        bool isSelected = rootNode == selected;
        if (isSelected) {
            flags |= ImGuiTreeNodeFlags_Selected;
        }
        bool open = ImGui::TreeNodeEx(name->name.c_str(), flags);
        if (ImGui::IsItemClicked()) {
            auto editorGameMode = dynamic_cast<voxie::EditorGameMode *>(voxie::Engine::GetEngine().GetGameMode());
            editorGameMode->SetSelection(rootNode);
        }
        if (ImGui::BeginDragDropSource()) {
            ImGui::SetDragDropPayload(IMGUI_PAYLOAD_TYPE_SCENENODE, NULL, 0);
            draggedNode = root;
            ImGui::Text("%s", name->name.c_str());
            ImGui::EndDragDropSource();
        }

        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_SCENENODE)) {
                draggedNode->MoveTo(root);
                draggedNode = nullptr;
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::TableNextColumn();
        return open;
    }

    void DisplayTreeView(voxie::Node *root, voxie::Handle &selected) {
        if (!root) {
            return;
        }
        bool isFolder = root->GetNumChildren() > 0;
        if (isFolder) {
            if (DisplayNode(root, selected, ImGuiTreeNodeFlags_SpanFullWidth)) {
                for (const auto &node : root->GetChildNodes()) {
                    DisplayTreeView(node.get(), selected);
                }
                ImGui::TreePop();
            }
        } else {
            DisplayNode(root, selected, ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);
            for (const auto &node : root->GetChildNodes()) {
                DisplayTreeView(node.get(), selected);
            }
        }
    }

    void ShowTreeView(voxie::Handle &selected) {
        static ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody;

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
        auto editorGameMode = dynamic_cast<voxie::EditorGameMode *>(voxie::Engine::GetEngine().GetGameMode());
        auto selected = editorGameMode->GetSelection();

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
        if (auto camera = voxie::Engine::GetEngine().GetCamera())
            camera->SetSelection(entity);

        AddNewComponent();
        ImGui::SameLine();
        RemoveComponent(entity);

        if (voxie::helper::HasComponent<voxie::Name>(entity)) {
            ShowEntityNameController(entity);
        }

        if (voxie::helper::HasComponent<voxie::VisibleText>(entity)) {
            ShowEntityVisibleTextController(entity);
        }

        if (voxie::helper::HasComponent<voxie::Position>(entity)) {
            ShowEntityPositionController(entity);
        }

        if (voxie::helper::HasComponent<voxie::RigidBody>(entity)) {
            ShowEntityBodyController(entity);
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

        if (voxie::helper::HasComponent<voxie::InvertedHull>(entity)) {
            ShowEntityInvertedHullController(entity);
        }

        if (voxie::helper::HasComponent<voxie::Attenuation>(entity)) {
            ShowEntityAttenuationController(entity);
        }

        if (voxie::helper::HasComponent<voxie::Material>(entity)) {
            ShowEntityMaterialController(entity);
        }

        if (voxie::helper::HasComponent<voxie::Direction>(entity)) {
            ShowCameraSelectorController(entity);
        }

        ImGui::End();
    }

}// namespace gui
