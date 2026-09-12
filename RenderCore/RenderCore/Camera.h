#pragma once

#include <glm/glm.hpp>

// ============================================================
// CAMERA
// ============================================================
//
// A simple top-down orthographic 2D camera. World units are
// kilometers (see GeoUtils.h). Supports panning and zooming so
// the user can navigate around shapes placed via lat/long.
//
class Camera
{
public:
    Camera();

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;

    void SetViewportSize(int widthPx, int heightPx);
    glm::vec2 GetViewportSize() const;

    // Pan by a delta expressed in screen pixels.
    void PanByPixels(glm::vec2 deltaPx);

    // Zoom in/out. factor > 1 zooms in, factor < 1 zooms out.
    void ZoomBy(float factor, glm::vec2 pivotScreenPx);

    void Reset();

    // Convert a widget-space pixel (origin top-left, y-down,
    // as Qt reports mouse positions) into world space on the
    // Z = 0 plane.
    glm::vec3 ScreenToWorld(glm::vec2 screenPx) const;

    float GetWorldUnitsPerPixel() const;

private:
    glm::vec2 m_center{0.0f, 0.0f};   // world-space center of the view
    float m_worldUnitsPerPixel = 0.01f; // zoom level (smaller = more zoomed in)

    int m_viewportWidth = 800;
    int m_viewportHeight = 600;
};
