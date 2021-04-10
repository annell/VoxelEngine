#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>


#include "Shader.h"
#include "Camera.h"
#include "Cube.h"

#include <iostream>

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

// lighting
glm::vec3 lightPos(1.2f, 3.0f, 2.0f);
glm::vec3 lightPos2(1.2f, 3.0f, 2.0f);

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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
    Shader lightingShader("/Users/stan/dev/C++/GraphicsLightning/shaders/basic_light.vs", "/Users/stan/dev/C++/GraphicsLightning/shaders/basic_light.fs");
    Shader lightCubeShader("/Users/stan/dev/C++/GraphicsLightning/shaders/light_cube.vs", "/Users/stan/dev/C++/GraphicsLightning/shaders/light_cube.fs");
    Shader lightCubeShader2("/Users/stan/dev/C++/GraphicsLightning/shaders/light_cube.vs", "/Users/stan/dev/C++/GraphicsLightning/shaders/light_cube.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    std::vector<Cube*> cubes;
    bool first = true;
    int nrCubes = 10;
    for (int i = 0; i < nrCubes; i++) {
        for (int n = 0; n < nrCubes; n++) {
            Cube* cube = new Cube(
                    { i * 0.11f, 2,  n * 0.11f},
                    {0.1, 0.1, 0.1},
                    {{1.0f, 1.0f, 1.0f},
                    {(float)i / nrCubes - 0.2, (float)n / nrCubes - 0.2, (float)(i + n)/nrCubes*2 - 0.2},
                    {1.0f, 0.5f, 0.31f},
                    {0.5f, 0.5f, 0.5f},
                    32.0f});
            cube->SetVertexAttrib(3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            cube->SetVertexAttrib(3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            cubes.push_back(cube);
        }
    }

    Cube floor({0, 0, 0},
               {1000000, 0, 1000000},
               {{0.2f, 0.8f, 0.31f},
                 {0.2f, 0.2f, 0.2f},
                 {1.0f, 0.5f, 0.31f},
                 {0.5f, 0.5f, 0.5f},
                 32.0f});
    floor.SetVertexAttrib(3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    floor.SetVertexAttrib(3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    Cube lightCube({0, 0, 0}, {1, 1, 1});
    lightCube.SetVertexAttrib(3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    Cube lightCube2({0, 0, 0}, {1, 1, 1});
    lightCube2.SetVertexAttrib(3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

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
            std::cout << "FPS: " << (int)(1/deltaTime) << std::endl;
            FPSUpdate = 10;
        }
        lastFrame = currentFrame;
        lightPos[0] = 1*sin((float)n /30);
        lightPos[2] = 1*cos((float)n /30);

        lightPos2[0] = 1*cos((float)n /40);
        lightPos2[2] = 1*sin((float)n /40);

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("objectColor", 0.0f, 1.0f, 0.31f);
        lightingShader.setInt("nrLights", 2);
        lightingShader.setVec3("lights[0].lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("lights[0].lightPos", lightPos);
        lightingShader.setVec3("lights[1].lightColor", 5.0f, 0.0f, 0.0f);
        lightingShader.setVec3("lights[1].lightPos", lightPos2);
        camera.SetShaderParameters(lightingShader);

        lightingShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        lightingShader.setFloat("material.shininess", 32.0f);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        //lightingShader.setMat4("model", model);
        for (auto& cube : cubes) {
            cube->SetShaderParameters(lightingShader);
            cube->Draw();
        }

        floor.SetShaderParameters(lightingShader);
        floor.Draw();

        // also draw the lamp object
        lightCubeShader.use();
        camera.SetShaderParameters(lightCubeShader);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.1f)); // a smaller cube
        lightCubeShader.setMat4("model", model);
        lightCube.Draw();

        lightCubeShader2.use();
        camera.SetShaderParameters(lightCubeShader2);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos2);
        model = glm::scale(model, glm::vec3(0.1f)); // a smaller cube
        lightCubeShader2.setMat4("model", model);
        lightCube2.Draw();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
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