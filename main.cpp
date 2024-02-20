#include "common.h"
#include "crosshair.h"
#include "shader.h"
#include "vao_vbo.h"
#include "camera.h"
#include "planet.h"
#include "texture.h"
#include "skybox.h"
#include "solar_system.h"


#include "glm/gtx/string_cast.hpp"

void checkGLError()
{
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        std::cerr << "Ошибка OpenGL: " << err << std::endl;
    }
}

struct winParams
{
    int WW, WH;
    int WX, WY;
    float aspectRatio;
};

struct WindowVars
{
    winParams prefs, curr;
    Camera camera;
    SolarSystem solarSystem;
    Crosshair crosshair;
    bool fullscreen = false;
    bool drawCrosshair = true;
    float size_scale = 0.00001f;
    float sun_size_scale = 0.0000005f;
    float orbit_scale = 0.00000001f;
    float time_scale = 1.0f;

    void UpdateWin(GLFWwindow* window)
    {
        glfwGetWindowSize(window, &curr.WW, &curr.WH);
        glfwGetWindowPos(window, &curr.WX, &curr.WY);
        curr.aspectRatio = static_cast<float>(curr.WW) / static_cast<float>(curr.WH);
    }

    WindowVars(GLFWwindow* window)
        : crosshair(window)
    {
        UpdateWin(window);

        camera = Camera(window, glm::vec3(-60, 150, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 45.0f, curr.aspectRatio, 0.001f, 100000.0f);
        solarSystem = SolarSystem(size_scale, sun_size_scale, orbit_scale);
    }
};

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    WindowVars* winvars = static_cast<WindowVars*>(glfwGetWindowUserPointer(window));
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        winvars->camera.ZoomTo(static_cast<float>(yoffset));
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
    {
        winvars->camera.ChangeSpeed(static_cast<float>(yoffset));
    }
    else
    {
        winvars->time_scale = static_cast<float>(yoffset) > 0 ? winvars->time_scale * 1.1f : winvars->time_scale / 1.1f;
        if (winvars->time_scale > 1000.f)
            winvars->time_scale = 1000.f;
        else if (winvars->time_scale < 0.01f)
            winvars->time_scale = 0.01f;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    WindowVars* winvars = static_cast<WindowVars*>(glfwGetWindowUserPointer(window));
    glViewport(0, 0, width, height);

    winvars->crosshair.Update(window);
    // Обновление соотношение сторон экрана
    float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    winvars->camera.UpdateAspectRatio(aspectRatio);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    WindowVars* winvars = static_cast<WindowVars*>(glfwGetWindowUserPointer(window));
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        float minDistance = std::numeric_limits<float>::max();
        int i = 0;

        // Перебор планет и проверка на какую сейчас смотрим
        for (auto& planet : winvars->solarSystem.GetPlanets())
        {
            float distance = winvars->camera.raySphereIntersection(winvars->camera.GetPosition(), winvars->camera.GetViewDirection(), planet.GetPosition(), planet.GetRadius());

            if (distance >= 0 && distance < minDistance)
            {
                minDistance = distance;
                winvars->camera.SetViewedPlanet(i);
            }

            i++;
        }
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
    WindowVars* winvars = static_cast<WindowVars*>(glfwGetWindowUserPointer(window));
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_P)
        {
            if (winvars->camera.GetViewedPlanet() > 0)
                winvars->solarSystem.GetPlanets().at(winvars->camera.GetViewedPlanet()).AddMoon();
        }
        else if (key == GLFW_KEY_TAB)
        {
            if (winvars->camera.GetViewedPlanet() >= 0)
                winvars->camera.SetViewedPlanet(winvars->camera.GetViewedPlanet() == 8 ? 0 : winvars->camera.GetViewedPlanet() + 1);
        }
        else if (key == GLFW_KEY_F1)
        {
            winvars->drawCrosshair = !winvars->drawCrosshair;
        }
        else if (key == GLFW_KEY_F2)
        {
            winvars->solarSystem.drawOrbitsOn = !winvars->solarSystem.drawOrbitsOn;
        }
        else if (key == GLFW_KEY_F11)
        {
            winvars->fullscreen = !winvars->fullscreen;
            if (winvars->fullscreen)
            {
                // Переход в полноэкранный режим
                winvars->UpdateWin(window);
                winvars->prefs = winvars->curr;
                GLFWmonitor* primary = glfwGetPrimaryMonitor();
                const GLFWvidmode* mode = glfwGetVideoMode(primary);
                glfwSetWindowMonitor(window, primary, 0, 0, mode->width, mode->height, mode->refreshRate);
            }
            else
            {
                // Переход в оконный режим
                glfwSetWindowMonitor(window, nullptr,
                    winvars->prefs.WX, winvars->prefs.WY,
                    winvars->prefs.WW, winvars->prefs.WH, GLFW_DONT_CARE);
            }
        }
    }
}

int main()
{
    // Инициализация GLFW
    if (!glfwInit())
    {
        std::cerr << "Не удалось инициализировать GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE); // Включение двойной буферизации

    // Создание окна GLFW
    GLFWwindow* window = glfwCreateWindow(1200, 700, "Солнечная система", nullptr, nullptr);

    if (!window)
    {
        std::cerr << "Не удалось создать окно GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetWindowPos(window, 100, 100);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, keyCallback);

    glfwWindowHint(GLFW_DEPTH_BITS, 32); // Установка точности буфера глубины на 32 бита
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE; // Последняя доступная версия OpenGL

    // Инициализация GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Не удалось инициализировать GLEW" << std::endl;
        return -1;
    }

    WindowVars* winvars = new WindowVars(window);

    // Настройка скайбокса
    Shader skyboxShader("shaders/sbVertexShader.vs", "shaders/fragmentShaderNoLight.fs");

    SkyBox skybox("images/stars.tga");

    // Установка пользовательского указателя окна на переменные окна
    glfwSetWindowUserPointer(window, winvars);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glm::vec3 lightPos(0.0f, 0.0f, 0.0f);  // Источник освещения в мировых координатах

    // Включение тестирования глубины
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float planetTime = 5000;
    float delta = 0;
    double prevtime = 0;

    // Цикл рендеринга
    while (!glfwWindowShouldClose(window))
    {
        delta = winvars->time_scale * static_cast<float>(glfwGetTime() - prevtime);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        planetTime += delta;

        // Проверка, на окне ли фокус и находится ли курсор в окне
        bool update = glfwGetWindowAttrib(window, GLFW_FOCUSED) && glfwGetWindowAttrib(window, GLFW_HOVERED);
        if (update)
        {
            // Обновление планет
            winvars->solarSystem.Update(planetTime);

            winvars->camera.ProcessInput(static_cast<float>(glfwGetTime() - prevtime), winvars->solarSystem.GetPlanets());
        }
        prevtime = glfwGetTime();

        skyboxShader.Use();
        glm::mat4 viewWithoutTranslation = glm::mat4(glm::mat3(winvars->camera.GetViewMatrix()));
        skyboxShader.SetMat4("view", viewWithoutTranslation);
        skyboxShader.SetMat4("projection", winvars->camera.GetProjectionMatrix());
        skybox.Draw(skyboxShader);

        if (winvars->drawCrosshair)
            winvars->crosshair.Draw(winvars->camera.GetCrosshairColor());

        glDepthFunc(GL_LESS);
        glDepthRange(0, 1);

        winvars->solarSystem.Draw(winvars->camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Очистка GLFW
    glfwTerminate();
    return 0;
}