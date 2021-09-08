//
// Created by Stefan Annell on 2021-05-09.
//

#include "MouseHandler.h"
#include "Camera.h"
#include "Engine.h"

namespace voxie {
    namespace internal {

        float lastX = 0;
        float lastY = 0;
        bool firstMouse = true;
        bool mouseLock = false;

    }// namespace internal

    void MouseHandler::framebuffer_size_callback(GLFWwindow*, int width, int height) {
        glViewport(0, 0, width, height);
        voxie::Engine::GetEngine().GetWindow()->SetWidth(width);
        voxie::Engine::GetEngine().GetWindow()->SetHeight(height);
    }

    void MouseHandler::mouse_callback(GLFWwindow*, double xpos, double ypos) {
        if (internal::mouseLock) {
            if (internal::firstMouse) {
                internal::lastX = xpos;
                internal::lastY = ypos;
                internal::firstMouse = false;
            }

            float xoffset = xpos - internal::lastX;
            float yoffset = internal::lastY - ypos;// reversed since y-coordinates go from bottom to top

            internal::lastX = xpos;
            internal::lastY = ypos;

            Engine::GetEngine().GetCamera()->ProcessMouseMovement(xoffset, yoffset);
        }
    }

    void MouseHandler::scroll_callback(GLFWwindow*, double, double) {
    }

    void MouseHandler::UnlockCamera() {
        glfwSetInputMode(Engine::GetEngine().GetWindow()->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        internal::mouseLock = false;
    }

    void MouseHandler::LockCamera() {
        glfwSetInputMode(Engine::GetEngine().GetWindow()->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (!internal::mouseLock) {
            internal::firstMouse = true;
        }
        internal::mouseLock = true;
    }

}// namespace voxie
