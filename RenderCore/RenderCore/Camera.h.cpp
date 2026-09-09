#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    Camera();
    void setOrtho(float left, float right, float bottom, float top, float near = -1.0f, float far = 1.0f);
    const glm::mat4& getProjectionMatrix() const { return m_projection; }
    const glm::mat4& getViewMatrix() const { return m_view; }

    // Convert screen coordinates (in pixels) to world coordinates
    glm::vec2 screenToWorld(const glm::vec2& screenPos, const glm::vec2& viewportSize) const;

private:
    glm::mat4 m_projection;
    glm::mat4 m_view;
};