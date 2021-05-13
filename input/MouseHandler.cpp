//
// Created by Stefan Annell on 2021-05-09.
//

#include "MouseHandler.h"
#include "Camera.h"
#include "../Engine.h"

namespace internal {

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool mouseLock = false;

}

void MouseHandler::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void MouseHandler::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (internal::mouseLock) {

        if (internal::firstMouse)
        {
            internal::lastX = xpos;
            internal::lastY = ypos;
            internal::firstMouse = false;
        }

        float xoffset = xpos - internal::lastX;
        float yoffset = internal::lastY - ypos; // reversed since y-coordinates go from bottom to top

        internal::lastX = xpos;
        internal::lastY = ypos;

        Engine::GetEngine().GetCamera()->ProcessMouseMovement(xoffset, yoffset);
    }
}

void MouseHandler::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Engine::GetEngine().GetCamera()->ProcessMouseScroll(yoffset);
}

void MouseHandler::UnlockCamera() {
    glfwSetInputMode(Engine::GetEngine().GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    internal::mouseLock = false;
}

void MouseHandler::LockCamera() {
    glfwSetInputMode(Engine::GetEngine().GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (!internal::mouseLock) {
        internal::firstMouse = true;
    }
    internal::mouseLock = true;
}

