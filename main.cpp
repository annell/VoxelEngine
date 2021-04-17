#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Cube.h"
#include "Lightsource.h"
#include "CubeHandler.h"
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

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    glewInit();

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("/Users/stan/dev/C++/VoxelEngine/shaders/basic_light.vs",
                          "/Users/stan/dev/C++/VoxelEngine/shaders/basic_light.fs");
    Shader lightCubeShader("/Users/stan/dev/C++/VoxelEngine/shaders/light_cube.vs",
                           "/Users/stan/dev/C++/VoxelEngine/shaders/light_cube.fs");


    int nrCubes = 100;
    CubeHandler cubeHandler(&lightingShader);
    ModelLoader::LoadModel("/Users/stan/dev/C++/VoxelEngine/voxelObjects/chr_sword.vox", cubeHandler);

    cubeHandler.AddCube(std::make_unique<Cube>(
            Position{0, -1, 0},
            Dimensions{1000, 0, 1000},
            Material{{0.2f, 0.8f, 0.31f},
                     {0.2f, 0.2f, 0.2f},
                     {1.0f, 0.5f, 0.31f},
                     {0.5f, 0.5f, 0.5f},
                     32.0f}, cubeHandler.NrMaterials() + 1));

    cubeHandler.Init();

    LightSourceHandler lights(&lightCubeShader, &lightingShader);

    lights.AddLight(LightSource(new Cube({1, 1, 1}, {1.0, 1.0, 1.0}), {0.6f, 0.6f, 0.6f}, {1.0f, 3.0f, 1.0f}));
    lights.AddLight(LightSource(new Cube({1, 1, 1}, {1.0, 1.0, 1.0}), {0.6f, 0.2f, 0.2f}, {2.0f, 3.0f, 2.0f}));

    std::cout << "NrVertex: " << cubeHandler.NrVertex() << std::endl;

    // render loop
    // -----------
    int n = 0;
    int FPSUpdate = 10;
    while (!glfwWindowShouldClose(window))
    {
        n++;
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        if (FPSUpdate-- <= 0) {
            //std::cout << "FPS: " << (int)(1/deltaTime) << std::endl;
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

        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lights.Draw(camera);
        cubeHandler.Draw(camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
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

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
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

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}