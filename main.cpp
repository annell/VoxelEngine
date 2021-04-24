#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Cube.h"
#include "Lightsource.h"
#include "Chunk.h"
#include "ModelLoader.h"

#include <iostream>
#include <memory>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 1.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "VoxelEngine", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewInit();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    Shader lightCubeShader("/Users/stan/dev/C++/VoxelEngine/shaders/light_cube.vs",
                           "/Users/stan/dev/C++/VoxelEngine/shaders/light_cube.fs");

    Chunk model(Shader("/Users/stan/dev/C++/VoxelEngine/shaders/basic_light.vs",
                          "/Users/stan/dev/C++/VoxelEngine/shaders/basic_light.fs"), {0, 0, 0});
    ModelLoader::LoadModel("/Users/stan/dev/C++/VoxelEngine/voxelObjects/monu10.vox", model);
    model.Init();

    Chunk model2(Shader("/Users/stan/dev/C++/VoxelEngine/shaders/basic_light.vs",
                          "/Users/stan/dev/C++/VoxelEngine/shaders/basic_light.fs"), {0, 0, 0});
    ModelLoader::LoadModel("/Users/stan/dev/C++/VoxelEngine/voxelObjects/monu10.vox", model2);
    model2.Init();

    Chunk floor(Shader("/Users/stan/dev/C++/VoxelEngine/shaders/basic_light.vs",
                          "/Users/stan/dev/C++/VoxelEngine/shaders/basic_light.fs"), {0, 0, 0});
    floor.AddCube(std::make_unique<Cube>(
            Position{0, -0.1, 0},
            Dimensions{10000, 0.1, 10000},
            Material{{0.0f, 0.0f, 0.0f},
                     {0.5f, 0.5f, 0.5f},
                     {0.5f, 0.5f, 0.5f},
                     {0.5f, 0.5f, 0.5f},
                     32.0f}, 0));
    floor.Init();

    std::vector<Shader*> shaders;
    shaders.push_back(model.GetShader());
    shaders.push_back(model2.GetShader());
    shaders.push_back(floor.GetShader());
    LightSourceHandler lights(&lightCubeShader, shaders);

    lights.AddLight(LightSource(new Cube({1, 1, 1}, {1.0, 1.0, 1.0}), 
                                {1.0f, 1.0f, 1.0f}, 
                                {1.0f, 1.0f, 1.0f}));

    std::cout << "NrVertex: " << model.NrVertex() << std::endl;

    int n = 0;
    int FPSUpdate = 10;
    while (!glfwWindowShouldClose(window)) {
        n++;
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        if (FPSUpdate-- <= 0) {
            std::cout << "FPS: " << (int)(1/deltaTime) << std::endl;
            FPSUpdate = 20;
        }
        lastFrame = currentFrame;
        bool first = true;
        for (auto& light : lights.GetLightSources()) {
            auto pos = light.GetPosition();
            if (first) {
                pos[0] = 1*sin((float)n / 40);
                pos[2] = 1*cos((float)n / 40);
                first = false;
            } else {
                pos[0] = 1*cos((float)n / 40);
                pos[2] = 1*sin((float)n / 40);
            }
            light.SetPosition(pos);
        }
        auto pos = model.GetPosition();
        pos.x = pos.x + 0.01;
        model.SetPosition(pos);
        pos = model2.GetPosition();
        pos.z = pos.z + 0.01;
        model2.SetPosition(pos);

        processInput(window);

        glClearColor(0.25f, 0.6f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lights.Draw(camera);
        model.Draw();
        model2.Draw();
        floor.Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}