#include "Main.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>
#include <iostream>

#include "Graphics.hpp"
#include "Raycaster.hpp"

 int map[] = {
     1,1,1,1,1,1,1,1,
     1,0,1,0,0,0,0,1,
     1,0,1,1,0,0,0,1,
     1,0,0,0,0,0,0,1,
     1,0,0,0,0,0,0,1,
     1,0,0,0,1,0,0,1,
     1,0,0,0,0,1,0,1,
     1,1,1,1,1,1,1,1,
 };

void Init()
{
	Graphics::Shader::Default()->Bind();
    glm::mat4 proj = glm::ortho(0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f, -1.0f, 1.0f);
	Graphics::Shader::Default()->SetUniformMat4f("u_MVP", proj);

    glClearColor(0.3f, 0.3f, 0.3f, 1);
}

void GLAPIENTRY
GLErrorCallback(GLenum source,
                GLenum type,
                GLuint id,
                GLenum severity,
                GLsizei length,
                const GLchar* message,
                const void* userParam)
{
    if (type != GL_DEBUG_TYPE_ERROR)
        return;

    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            "** GL ERROR **", type, severity, message);
    __debugbreak();
}

int main()
{
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Raycaster", nullptr, nullptr);
    if (window == nullptr)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        return -1;

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(GLErrorCallback, nullptr);

    Init();

    float lastTime = 0, t = 0, deltaTime = 0;
    Graphics::Renderer renderer;
    Raycaster raycaster(renderer, *map);
    Player player(MAP_S * 4, MAP_S * 4);
    while (!glfwWindowShouldClose(window))
    {
        t = glfwGetTime();
        deltaTime = t - lastTime;
        lastTime = t;

        glClear(GL_COLOR_BUFFER_BIT);

        player.Update(*window, deltaTime);
        
        renderer.BeginDraw(*Graphics::Shader::Default());

        raycaster.Draw(player);
        player.Draw(renderer);

        renderer.EndDraw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    delete Graphics::Shader::Default();

    glfwDestroyWindow(window);
    glfwTerminate();

    if (_CrtDumpMemoryLeaks())
        std::cout << "Memory leeeeek" << std::endl;

    return 0;
}
