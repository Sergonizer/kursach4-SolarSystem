#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"
#include "planet.h"

class Camera
{
public:
    static float raySphereIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const glm::vec3& sphereCenter, float sphereRadius)
    {
        glm::vec3 oc = rayOrigin - sphereCenter;
        float a = glm::dot(rayDirection, rayDirection);
        float b = 2.0f * glm::dot(oc, rayDirection);
        float c = glm::dot(oc, oc) - sphereRadius * sphereRadius;
        float discriminant = b * b - 4 * a * c;

        if (discriminant < 0)
        {
            return -1.0f; // Пересечений нет
        }
        else
        {
            // Нахождение ближайшей точки пересечения
            float t1 = (-b - sqrt(discriminant)) / (2.0f * a);
            float t2 = (-b + sqrt(discriminant)) / (2.0f * a);

            // Проверка, находится ли точка пересечения перед лучом
            float t = (t1 < t2) ? t1 : t2;
            if (t >= 0)
            {
                return t;
            }
            else
            {
                return -1.0f; // Пересечений впереди нет
            }
        }
    }

    Camera() = default;
    Camera(GLFWwindow* window, glm::vec3 position, glm::vec3 worldUp, GLfloat yaw, GLfloat pitch, GLfloat fieldOfView, GLfloat aspectRatio, GLfloat nearPlane, GLfloat farPlane);

    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();
    glm::vec3 GetViewDirection();
    glm::vec3 GetPosition() { return position; }
    glm::vec4 GetCrosshairColor() { return crosshairColorValue; }
    int GetViewedPlanet() { return observingPlanetIndex; }
    void SetViewedPlanet(int viewedPlanet) { observingPlanetIndex = viewedPlanet; }
    void ProcessInput(float delta, std::vector<Planet> planets);
    void LookAtPoint(glm::vec3 pos);
    void ZoomTo(float val);
    void ChangeSpeed(float val);
    void UpdateAspectRatio(float ar)
    {
        if (std::isnan(ar))
            return;
        aspectRatio = ar;
    }

    void OrbitAroundPoint(glm::vec3 center, float radius, float horizontalAngle, float verticalAngle);

private:
    GLFWwindow* window;

    int observingPlanetIndex;
    float horizontalAngle, verticalAngle;
    float zoom;

    glm::vec4 crosshairColorValue;

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    GLfloat yaw;
    GLfloat pitch;

    GLfloat movementSpeed;
    GLfloat sensitivity;

    GLfloat fieldOfView;
    GLfloat aspectRatio;
    GLfloat nearPlane;
    GLfloat farPlane;

    void UpdateCameraVectors();
};

#endif