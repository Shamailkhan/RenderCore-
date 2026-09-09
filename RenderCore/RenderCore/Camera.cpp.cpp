#include "Camera.h"

Camera::Camera()
{
    m_view = glm::mat4(1.0f);
}

void Camera::setOrtho(float left, float right, float bottom, float top, float near, float far)
{
    m_projection = glm::ortho(left, right, bottom, top, near, far);
}

glm::vec2 Camera::screenToWorld(const glm::vec2& screenPos, const glm::vec2& viewportSize) const
{
    // Convert from [0, viewportSize] to NDC [-1,1]
    glm::vec2 ndc;
    ndc.x = (2.0f * screenPos.x / viewportSize.x) - 1.0f;
    ndc.y = 1.0f - (2.0f * screenPos.y / viewportSize.y); // flip y

    // Inverse of orthographic projection * view (view is identity)
    // For ortho, world = ndc * (right-left)/2 + center
    // We'll use inverse projection matrix.
    glm::mat4 invProj = glm::inverse(m_projection);
    glm::vec4 world = invProj * glm::vec4(ndc, 0.0f, 1.0f);
    return glm::vec2(world.x, world.y);
}