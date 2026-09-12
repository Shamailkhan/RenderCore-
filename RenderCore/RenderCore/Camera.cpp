#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>


Camera::Camera() = default;


// ============================================================
// VIEW / PROJECTION
// ============================================================

glm::mat4 Camera::GetViewMatrix() const
{
    // Looking straight down the Z axis at the world plane.
    const glm::vec3 eye(m_center.x, m_center.y, 10.0f);
    const glm::vec3 target(m_center.x, m_center.y, 0.0f);
    const glm::vec3 up(0.0f, 1.0f, 0.0f);

    return glm::lookAt(eye, target, up);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
    const float halfWidth  = 0.5f * m_viewportWidth  * m_worldUnitsPerPixel;
    const float halfHeight = 0.5f * m_viewportHeight * m_worldUnitsPerPixel;

    return glm::ortho(
        -halfWidth, halfWidth,
        -halfHeight, halfHeight,
        0.01f, 100.0f
    );
}


// ============================================================
// VIEWPORT
// ============================================================

void Camera::SetViewportSize(int widthPx, int heightPx)
{
    m_viewportWidth  = std::max(1, widthPx);
    m_viewportHeight = std::max(1, heightPx);
}

glm::vec2 Camera::GetViewportSize() const
{
    return glm::vec2(
        static_cast<float>(m_viewportWidth),
        static_cast<float>(m_viewportHeight)
    );
}


// ============================================================
// PAN / ZOOM
// ============================================================

void Camera::PanByPixels(glm::vec2 deltaPx)
{
    // Screen Y is down, world Y is up -> flip Y.
    m_center.x -= deltaPx.x * m_worldUnitsPerPixel;
    m_center.y += deltaPx.y * m_worldUnitsPerPixel;
}

void Camera::ZoomBy(float factor, glm::vec2 pivotScreenPx)
{
    if (factor <= 0.0f)
        return;

    // Keep the world point under the pivot stationary while zooming.
    const glm::vec3 worldBefore = ScreenToWorld(pivotScreenPx);

    m_worldUnitsPerPixel = std::clamp(
        m_worldUnitsPerPixel / factor,
        0.00001f,
        1000.0f
    );

    const glm::vec3 worldAfter = ScreenToWorld(pivotScreenPx);

    m_center.x -= (worldAfter.x - worldBefore.x);
    m_center.y -= (worldAfter.y - worldBefore.y);
}

void Camera::Reset()
{
    m_center = glm::vec2(0.0f, 0.0f);
    m_worldUnitsPerPixel = 0.01f;
}


// ============================================================
// SCREEN <-> WORLD
// ============================================================

glm::vec3 Camera::ScreenToWorld(glm::vec2 screenPx) const
{
    const float ndcX = (screenPx.x / static_cast<float>(m_viewportWidth))  * 2.0f - 1.0f;
    const float ndcY = 1.0f - (screenPx.y / static_cast<float>(m_viewportHeight)) * 2.0f;

    const float halfWidth  = 0.5f * m_viewportWidth  * m_worldUnitsPerPixel;
    const float halfHeight = 0.5f * m_viewportHeight * m_worldUnitsPerPixel;

    const float worldX = m_center.x + ndcX * halfWidth;
    const float worldY = m_center.y + ndcY * halfHeight;

    return glm::vec3(worldX, worldY, 0.0f);
}

float Camera::GetWorldUnitsPerPixel() const
{
    return m_worldUnitsPerPixel;
}
