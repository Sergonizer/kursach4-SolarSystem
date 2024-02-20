#include "camera.h"

Camera::Camera(GLFWwindow* window, glm::vec3 position, glm::vec3 worldUp, GLfloat yaw, GLfloat pitch, GLfloat fieldOfView, GLfloat aspectRatio, GLfloat nearPlane, GLfloat farPlane)
    : window(window), front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(5.f), sensitivity(0.1f),
    position(position), worldUp(worldUp), yaw(yaw), pitch(pitch), fieldOfView(fieldOfView), aspectRatio(aspectRatio),
    nearPlane(nearPlane), farPlane(farPlane), observingPlanetIndex(3),
    horizontalAngle(0), verticalAngle(0), zoom(1)
{
    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::GetProjectionMatrix()
{
    return glm::perspective(glm::radians(fieldOfView), aspectRatio, nearPlane, farPlane);
}

glm::vec3 Camera::GetViewDirection()
{
    return front;
}

void Camera::ProcessInput(float delta, std::vector<Planet> planets)
{
    // ��������� ����� � ����
    int WW, WH;
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    glfwGetWindowSize(window, &WW, &WH);

    // ���������� �������� �� ������ ����
    float xoffset = static_cast<float>(xpos) - static_cast<float>(WW) / 2;
    float yoffset = static_cast<float>(WH) / 2 - static_cast<float>(ypos);

    // ����� ��������� ������� � ����� ����
    glfwSetCursorPos(window, WW / 2, WH / 2);

    // ��������������� �������� ��� �������� ����������������
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        observingPlanetIndex = -1;
    else if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
        observingPlanetIndex = 0;
    else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        observingPlanetIndex = 1;
    else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        observingPlanetIndex = 2;
    else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        observingPlanetIndex = 3;
    else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        observingPlanetIndex = 4;
    else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        observingPlanetIndex = 5;
    else if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
        observingPlanetIndex = 6;
    else if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
        observingPlanetIndex = 7;
    else if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
        observingPlanetIndex = 8;

    if (observingPlanetIndex >= 0 && observingPlanetIndex < 9)
    {
        // ���������� ������ ��������� ������ ��� ������
        float orbitRadius = (1 / zoom) * 4.0f * planets.at(observingPlanetIndex).GetRadius();
        horizontalAngle += xoffset;  // ������������� ���� ������ �� �����������
        verticalAngle += yoffset;    // ������������� ���� ������ �� ���������

        // ��������� ����������� ��� ������������� ����
        float minVerticalAngle = -89.9f;  // ��������� ������� ����� ����� ��� �������������� ��������������� ������
        float maxVerticalAngle = 89.9f;   // ��������� ������� ����� ���� ��� �������������� ��������������� ������

        // ����������� ������������� ���� ��������� ����������
        verticalAngle = glm::clamp(verticalAngle, minVerticalAngle, maxVerticalAngle);

        OrbitAroundPoint(planets.at(observingPlanetIndex).GetPosition(), orbitRadius, horizontalAngle, verticalAngle);
    }
    else
    {
        // ��������� ����� � ����������
        float ctrl = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ? 3.f : 1.f;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            position += ctrl * delta * movementSpeed * front;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            position -= ctrl * delta * movementSpeed * front;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            position -= ctrl * delta * movementSpeed * right;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            position += ctrl * delta * movementSpeed * right;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            position += ctrl * delta * movementSpeed * worldUp;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            position -= ctrl * delta * movementSpeed * worldUp;

        // ���������� �������� � ������� �� ������ ����� � ����
        yaw += xoffset;
        pitch += yoffset;

        // ����������� ������� ��� �������������� ��������������� ������
        if (pitch > 89.9f) pitch = 89.9f;
        if (pitch < -89.9f) pitch = -89.9f;
        UpdateCameraVectors();
    }

    float minDistance = std::numeric_limits<float>::max();
    int i = 0;
    crosshairColorValue = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);

    // ������� ������ � �������� �� ��, �� ����� ������ �������
    for (auto& planet : planets)
    {
        float distance = raySphereIntersection(GetPosition(), GetViewDirection(), planet.GetPosition(), planet.GetRadius());

        if (distance >= 0 && distance < minDistance)
        {
            minDistance = distance;
            crosshairColorValue = planet.GetTexture().GetAverageColor();
        }

        i++;
    }
}

void Camera::LookAtPoint(glm::vec3 pos)
{
    front = glm::normalize(pos - position);

    // ���������� �������� � ������� �� ������ ������� ������
    yaw = glm::degrees(atan2(front.z, front.x));
    pitch = glm::degrees(asin(front.y));

    // ���������� �������� ������
    UpdateCameraVectors();
}

void Camera::ZoomTo(float val)
{
    zoom *= val > 0 ? 1.1f : 1 / 1.1f;
    if (zoom > 1.0f)
        zoom = 1.0f;
    else if (zoom < 0.1f)
        zoom = 0.1f;
}

void Camera::ChangeSpeed(float val)
{
    movementSpeed *= val > 0 ? 1.1f : 1 / 1.1f;
    if (movementSpeed > 100.0f)
        movementSpeed = 100.0f;
    else if (movementSpeed < 0.01f)
        movementSpeed = 0.01f;
}

void Camera::OrbitAroundPoint(glm::vec3 center, float radius, float horizontalAngle, float verticalAngle)
{
    // ���������� ������ ��������� � �������������� ����������� ���������
    position.x = center.x + radius * glm::cos(glm::radians(horizontalAngle)) * glm::cos(glm::radians(verticalAngle));
    position.y = center.y + radius * glm::sin(glm::radians(verticalAngle));
    position.z = center.z + radius * glm::sin(glm::radians(horizontalAngle)) * glm::cos(glm::radians(verticalAngle));

    LookAtPoint(center);
}

void Camera::UpdateCameraVectors()
{
    // ���������� ����� �������� ������, ����� � �����
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}