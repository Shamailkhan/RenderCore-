#include "ShapeProvider.h"

#include <glm/gtc/constants.hpp>

#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QDebug>

#include <cmath>

// ============================================================
// GLOBALS EXPECTED BY LineRenderer.cpp
// ============================================================
//
// LineRenderer.cpp declares:
//     extern ShaderManager* g_shaderManager;
//     extern Camera* g_camera;
//
// Their single definitions live here, pointing at this
// ShapeProvider's own Camera/ShaderManager instances. The app only
// ever creates one ShapeProvider (owned by GLCanvas), so a single
// global pair is sufficient.
//
ShaderManager* g_shaderManager = nullptr;
Camera*        g_camera        = nullptr;


// ============================================================
// CONSTRUCTOR / DESTRUCTOR
// ============================================================

ShapeProvider::ShapeProvider(QObject* parent)
    : QObject(parent)
{
}

ShapeProvider::~ShapeProvider()
{
    if (g_shaderManager == &m_shaderManager)
        g_shaderManager = nullptr;

    if (g_camera == &m_camera)
        g_camera = nullptr;
}


// ============================================================
// GL LIFECYCLE
// ============================================================

void ShapeProvider::Initialize()
{
    glewExperimental = GL_TRUE;

    const GLenum glewStatus = glewInit();

    // glewInit() commonly raises a spurious GL_INVALID_ENUM under
    // core profiles; clear it so it doesn't get misattributed to
    // later GL calls.
    glGetError();

    if (glewStatus != GLEW_OK)
    {
        qWarning() << "ShapeProvider: glewInit failed -"
                   << reinterpret_cast<const char*>(glewGetErrorString(glewStatus));
        return;
    }

    g_shaderManager = &m_shaderManager;
    g_camera        = &m_camera;

    if (!m_shaderManager.LoadLineShader())
    {
        qWarning() << "ShapeProvider: failed to load LineRenderer shader program";
    }

    m_lineRenderer.Init();

    glClearColor(0.11f, 0.12f, 0.14f, 1.0f);

    UpdateStatusMessage();
}

void ShapeProvider::Resize(int w, int h)
{
    m_camera.SetViewportSize(w, h);
    glViewport(0, 0, w, h);
}

void ShapeProvider::Paint()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ------------------------------------------------------------
    // Live preview of the shape currently being drawn.
    //
    // Rather than switching on m_currentType here, build a
    // temporary Shape for whatever is in progress and run it
    // through the exact same BuildShapeVertices() that the
    // finalized shapes in m_shapes use (see PushShapeIntoRenderer).
    // That keeps the type -> vertices logic in one place instead of
    // duplicated per call site.
    //
    // This goes through AddLineStrip() (the DYNAMIC path), which
    // LineRenderer already clears at the end of every Render() call
    // - exactly what we want for a per-frame preview.
    // ------------------------------------------------------------
    if (m_isDrawing)
    {
        const Shape previewShape = BuildPreviewShape();

        // Polygons stay open while still being drawn - only close
        // the loop once FinalizePolygonIfPossible() actually adds it
        // to m_shapes.
        const bool closeLoop = (m_currentType != ShapeType::Polygon);

        const std::vector<glm::vec3> vertices = BuildShapeVertices(previewShape, closeLoop);

        if (!vertices.empty())
        {
            m_lineRenderer.AddLineStrip(vertices, CurrentColorVec(), m_currentThickness);
        }
    }

    m_lineRenderer.Render();
}

Shape ShapeProvider::BuildPreviewShape() const
{
    Shape shape;
    shape.type = m_currentType;
    shape.color = m_currentColor;
    shape.thickness = m_currentThickness;

    if (m_activePoints.empty())
        return shape;

    switch (m_currentType)
    {
        case ShapeType::Line:
        case ShapeType::Rectangle:
            shape.points = { m_activePoints[0], m_previewPoint };
            break;

        case ShapeType::Circle:
            shape.points = { m_activePoints[0] };
            shape.radius = glm::length(glm::vec2(m_previewPoint) - glm::vec2(m_activePoints[0]));
            break;

        case ShapeType::Polygon:
            shape.points = m_activePoints;
            shape.points.push_back(m_previewPoint);
            break;
    }

    return shape;
}


// ============================================================
// TOOL CONFIGURATION
// ============================================================

void ShapeProvider::SetShapeType(ShapeType type)
{
    if (m_isDrawing && type != m_currentType)
        CancelActiveDraw();

    m_currentType = type;
    UpdateStatusMessage();
}

void ShapeProvider::SetColor(const QColor& color)
{
    m_currentColor = color;
}

void ShapeProvider::SetThickness(float thickness)
{
    m_currentThickness = thickness;
}

void ShapeProvider::SetLabel(const QString& label)
{
    m_currentLabel = label;
}

void ShapeProvider::CancelActiveDraw()
{
    m_isDrawing = false;
    m_activePoints.clear();
    emit repaintNeeded();
    emit statusMessage(QStringLiteral("Cancelled."));
}


// ============================================================
// SHAPE LIST MANAGEMENT
// ============================================================

int ShapeProvider::AddFinalizedShape(Shape shape)
{
    PushShapeIntoRenderer(shape);

    m_shapes.push_back(shape);
    const int index = static_cast<int>(m_shapes.size()) - 1;

    emit repaintNeeded();
    emit shapeAdded(m_shapes.back(), index);

    return index;
}

void ShapeProvider::RemoveShapeAt(int index)
{
    if (index < 0 || index >= static_cast<int>(m_shapes.size()))
        return;

    m_shapes.erase(m_shapes.begin() + index);

    RebuildStaticBufferFromShapes();

    emit repaintNeeded();
    emit shapesReset();
}

void ShapeProvider::ClearAllShapes()
{
    m_shapes.clear();
    m_lineRenderer.ClearStaticLines();

    emit repaintNeeded();
    emit shapesReset();
}

void ShapeProvider::RebuildStaticBufferFromShapes()
{
    // LineRenderer's static buffer is append-only, so a single-row
    // removal is implemented by clearing everything and re-adding
    // whatever is left in m_shapes.
    m_lineRenderer.ClearStaticLines();

    for (const Shape& shape : m_shapes)
        PushShapeIntoRenderer(shape);
}

void ShapeProvider::PushShapeIntoRenderer(const Shape& shape)
{
    // This is now the ONLY place that turns a shape's points into
    // the actual line list handed to LineRenderer - RebuildStaticBufferFromShapes()
    // above iterates m_shapes and calls this once per entry, and
    // Paint()'s live preview reuses the same BuildShapeVertices()
    // helper on a temporary Shape instead of duplicating this switch.
    const glm::vec4 color(
        shape.color.redF(),
        shape.color.greenF(),
        shape.color.blueF(),
        shape.color.alphaF()
    );

    const std::vector<glm::vec3> vertices = BuildShapeVertices(shape, /*closeLoop=*/true);

    if (!vertices.empty())
    {
        m_lineRenderer.AddStaticLineStrip(vertices, color, shape.thickness);
    }
}

std::vector<glm::vec3> ShapeProvider::BuildShapeVertices(const Shape& shape, bool closeLoop) const
{
    switch (shape.type)
    {
        case ShapeType::Line:
        {
            if (shape.points.size() < 2)
                return {};

            return { shape.points[0], shape.points[1] };
        }

        case ShapeType::Rectangle:
        {
            if (shape.points.size() < 2)
                return {};

            return BuildRectangleLoop(shape.points[0], shape.points[1]);
        }

        case ShapeType::Circle:
        {
            if (shape.points.empty())
                return {};

            return BuildCircleLoop(shape.points[0], shape.radius);
        }

        case ShapeType::Polygon:
        {
            if (shape.points.size() < 2)
                return {};

            std::vector<glm::vec3> loop = shape.points;

            if (closeLoop)
                loop.push_back(shape.points.front());

            return loop;
        }
    }

    return {};
}


// ============================================================
// MOUSE / KEYBOARD INTERACTION
// ============================================================

void ShapeProvider::HandleMousePress(QMouseEvent* event)
{
    // ------------------------------------------------------------
    // Middle button: pan
    // ------------------------------------------------------------
    if (event->button() == Qt::MiddleButton)
    {
        m_isPanning = true;
        m_lastPanPos = event->pos();
        return;
    }

    // ------------------------------------------------------------
    // Right button: finalize a polygon (if one is in progress)
    // ------------------------------------------------------------
    if (event->button() == Qt::RightButton)
    {
        FinalizePolygonIfPossible();
        return;
    }

    if (event->button() != Qt::LeftButton)
        return;

    const glm::vec3 worldPos = WidgetPosToWorld(event->pos());

    switch (m_currentType)
    {
        case ShapeType::Line:
        case ShapeType::Rectangle:
        {
            if (!m_isDrawing)
            {
                m_isDrawing = true;
                m_activePoints = { worldPos };
                m_previewPoint = worldPos;
                emit statusMessage(QStringLiteral("Click the second point to finish."));
            }
            else
            {
                Shape shape;
                shape.type = m_currentType;
                shape.label = m_currentLabel;
                shape.color = m_currentColor;
                shape.thickness = m_currentThickness;
                shape.points = { m_activePoints[0], worldPos };

                m_isDrawing = false;
                m_activePoints.clear();

                AddFinalizedShape(shape);
                UpdateStatusMessage();
            }
            break;
        }

        case ShapeType::Circle:
        {
            if (!m_isDrawing)
            {
                m_isDrawing = true;
                m_activePoints = { worldPos };
                m_previewPoint = worldPos;
                emit statusMessage(QStringLiteral("Move to set the radius, click to finish."));
            }
            else
            {
                const glm::vec3& center = m_activePoints[0];
                const float radius = glm::length(glm::vec2(worldPos) - glm::vec2(center));

                Shape shape;
                shape.type = ShapeType::Circle;
                shape.label = m_currentLabel;
                shape.color = m_currentColor;
                shape.thickness = m_currentThickness;
                shape.points = { center };
                shape.radius = radius;

                m_isDrawing = false;
                m_activePoints.clear();

                AddFinalizedShape(shape);
                UpdateStatusMessage();
            }
            break;
        }

        case ShapeType::Polygon:
        {
            if (!m_isDrawing)
            {
                m_isDrawing = true;
                m_activePoints = { worldPos };
            }
            else
            {
                m_activePoints.push_back(worldPos);
            }

            m_previewPoint = worldPos;
            emit statusMessage(QStringLiteral(
                "Click to add another point, right-click (or double-click) to close the polygon."
            ));
            break;
        }
    }

    emit repaintNeeded();
}

void ShapeProvider::HandleMouseDoubleClick(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && m_currentType == ShapeType::Polygon)
        FinalizePolygonIfPossible();
}

void ShapeProvider::FinalizePolygonIfPossible()
{
    if (!m_isDrawing || m_currentType != ShapeType::Polygon)
        return;

    if (m_activePoints.size() < 2)
    {
        // Not enough points for a meaningful polygon yet - just cancel.
        CancelActiveDraw();
        return;
    }

    Shape shape;
    shape.type = ShapeType::Polygon;
    shape.label = m_currentLabel;
    shape.color = m_currentColor;
    shape.thickness = m_currentThickness;
    shape.points = m_activePoints;

    m_isDrawing = false;
    m_activePoints.clear();

    AddFinalizedShape(shape);
    UpdateStatusMessage();
}

void ShapeProvider::HandleMouseMove(QMouseEvent* event)
{
    if (m_isPanning)
    {
        const QPoint delta = event->pos() - m_lastPanPos;
        m_camera.PanByPixels(glm::vec2(delta.x(), delta.y()));
        m_lastPanPos = event->pos();
        emit repaintNeeded();
        return;
    }

    if (m_isDrawing)
    {
        m_previewPoint = WidgetPosToWorld(event->pos());
        emit repaintNeeded();
    }
}

void ShapeProvider::HandleWheel(QWheelEvent* event)
{
    const float steps = event->angleDelta().y() / 120.0f;
    const float zoomFactor = std::pow(1.15f, steps);

    const QPointF pos = event->position();
    m_camera.ZoomBy(zoomFactor, glm::vec2(pos.x(), pos.y()));

    emit repaintNeeded();
}

void ShapeProvider::HandleKeyPress(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape)
    {
        CancelActiveDraw();
        return;
    }

    if ((event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) &&
        m_currentType == ShapeType::Polygon)
    {
        FinalizePolygonIfPossible();
    }
}


// ============================================================
// HELPERS
// ============================================================

glm::vec3 ShapeProvider::WidgetPosToWorld(const QPoint& pos) const
{
    return m_camera.ScreenToWorld(glm::vec2(pos.x(), pos.y()));
}

glm::vec4 ShapeProvider::CurrentColorVec() const
{
    return glm::vec4(
        m_currentColor.redF(),
        m_currentColor.greenF(),
        m_currentColor.blueF(),
        m_currentColor.alphaF()
    );
}

void ShapeProvider::UpdateStatusMessage()
{
    QString msg;

    switch (m_currentType)
    {
        case ShapeType::Line:
            msg = QStringLiteral("Line tool: click a start point, then click an end point.");
            break;
        case ShapeType::Rectangle:
            msg = QStringLiteral("Rectangle tool: click one corner, then the opposite corner.");
            break;
        case ShapeType::Circle:
            msg = QStringLiteral("Circle tool: click the center, move to size it, click to finish.");
            break;
        case ShapeType::Polygon:
            msg = QStringLiteral("Polygon tool: click to add vertices, right-click/double-click/Enter to close.");
            break;
    }

    emit statusMessage(msg);
}

std::vector<glm::vec3> ShapeProvider::BuildRectangleLoop(
    const glm::vec3& cornerA,
    const glm::vec3& cornerB)
{
    const float minX = std::min(cornerA.x, cornerB.x);
    const float maxX = std::max(cornerA.x, cornerB.x);
    const float minY = std::min(cornerA.y, cornerB.y);
    const float maxY = std::max(cornerA.y, cornerB.y);

    return {
        glm::vec3(minX, minY, 0.0f),
        glm::vec3(maxX, minY, 0.0f),
        glm::vec3(maxX, maxY, 0.0f),
        glm::vec3(minX, maxY, 0.0f),
        glm::vec3(minX, minY, 0.0f) // close the loop
    };
}

std::vector<glm::vec3> ShapeProvider::BuildCircleLoop(
    const glm::vec3& center,
    float radius,
    int segments)
{
    std::vector<glm::vec3> points;
    points.reserve(static_cast<size_t>(segments) + 1);

    for (int i = 0; i <= segments; ++i)
    {
        const float t = (static_cast<float>(i) / static_cast<float>(segments)) * 2.0f * glm::pi<float>();

        points.emplace_back(
            center.x + radius * std::cos(t),
            center.y + radius * std::sin(t),
            center.z
        );
    }

    return points;
}
