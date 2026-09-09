#include "GLWidget.h"
#include <QMouseEvent>
#include <QDebug>
#include <cmath>

// Shader sources (as strings)
static const char* vertexShaderSource = R"(
#version 450 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in float thickness;
uniform mat4 u_mvp;
out VS_OUT {
    vec4 color;
    float thickness;
} vs_out;
void main() {
    gl_Position = u_mvp * vec4(position, 1.0);
    vs_out.color = color;
    vs_out.thickness = thickness;
}
)";

static const char* geometryShaderSource = R"(
#version 450 core
layout(lines) in;
layout(triangle_strip, max_vertices = 4) out;
in VS_OUT {
    vec4 color;
    float thickness;
} gs_in[];
out GS_OUT {
    vec4 color;
} gs_out;
uniform vec2 u_viewportSize;
void main() {
    vec4 p0 = gl_in[0].gl_Position;
    vec4 p1 = gl_in[1].gl_Position;
    vec2 ndc0 = p0.xy / p0.w;
    vec2 ndc1 = p1.xy / p1.w;
    vec2 direction = ndc1 - ndc0;
    float len = length(direction);
    if (len < 0.000001) return;
    direction /= len;
    vec2 normal = vec2(-direction.y, direction.x);
    float thickness = (gs_in[0].thickness + gs_in[1].thickness) * 0.5;
    vec2 pixelToNDC = vec2(2.0 / u_viewportSize.x, 2.0 / u_viewportSize.y);
    vec2 offset = normal * thickness * 0.5 * pixelToNDC;
    vec2 p0Plus = ndc0 + offset;
    vec2 p0Minus = ndc0 - offset;
    vec2 p1Plus = ndc1 + offset;
    vec2 p1Minus = ndc1 - offset;
    gs_out.color = gs_in[0].color;
    gl_Position = vec4(p0Plus * p0.w, p0.z, p0.w); EmitVertex();
    gs_out.color = gs_in[0].color;
    gl_Position = vec4(p0Minus * p0.w, p0.z, p0.w); EmitVertex();
    gs_out.color = gs_in[1].color;
    gl_Position = vec4(p1Plus * p1.w, p1.z, p1.w); EmitVertex();
    gs_out.color = gs_in[1].color;
    gl_Position = vec4(p1Minus * p1.w, p1.z, p1.w); EmitVertex();
    EndPrimitive();
}
)";

static const char* fragmentShaderSource = R"(
#version 450 core
in GS_OUT {
    vec4 color;
} fs_in;
out vec4 FragColor;
void main() {
    FragColor = fs_in.color;
}
)";

GLWidget::GLWidget(QWidget* parent)
    : QOpenGLWidget(parent),
      m_currentShapeType(Line),
      m_currentColor(Qt::white),
      m_currentThickness(1.0f),
      m_isDrawing(false)
{
    setFocusPolicy(Qt::StrongFocus);
}

GLWidget::~GLWidget()
{
    makeCurrent();
    // cleanup handled by destructors
    doneCurrent();
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    // Compile shader
    if (!m_shader.compile(vertexShaderSource, geometryShaderSource, fragmentShaderSource)) {
        qCritical() << "Failed to compile shader program!";
        return;
    }

    // Init line renderer
    m_lineRenderer.Init();

    // Setup camera (orthographic projection)
    // We'll set it later in resizeGL
    m_camera.setOrtho(-1, 1, -1, 1);
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    // Keep aspect ratio: we want world coordinates to be in meters, but we'll map to a reasonable range
    float aspect = (float)w / (float)h;
    float viewSize = 10.0f; // world units from center to edge
    if (aspect >= 1.0f)
        m_camera.setOrtho(-viewSize * aspect, viewSize * aspect, -viewSize, viewSize);
    else
        m_camera.setOrtho(-viewSize, viewSize, -viewSize / aspect, viewSize / aspect);
}

void GLWidget::paintGL()
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Add preview lines to dynamic buffer
    updatePreviewLines();

    // Render
    glm::mat4 mvp = m_camera.getProjectionMatrix() * m_camera.getViewMatrix();
    glm::vec2 viewportSize(width(), height());
    m_lineRenderer.Render(mvp, viewportSize, &m_shader);
}

void GLWidget::setShapeType(ShapeType type) { m_currentShapeType = type; }
void GLWidget::setColor(const QColor& color) { m_currentColor = color; }
void GLWidget::setThickness(float thickness) { m_currentThickness = thickness; }
void GLWidget::setLabel(const QString& label) { m_currentLabel = label; }
void GLWidget::setAnchor(const glm::vec2& anchor) { m_anchor = anchor; }
glm::vec2 GLWidget::getAnchor() const { return m_anchor; }

glm::vec2 GLWidget::mapToWorld(const QPoint& pos) const
{
    return m_camera.screenToWorld(glm::vec2(pos.x(), pos.y()), glm::vec2(width(), height()));
}

void GLWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton) return;

    glm::vec2 worldPos = mapToWorld(event->pos());
    glm::vec3 pos3(worldPos.x, worldPos.y, 0.0f);

    // Update anchor to this click position
    m_anchor = worldPos;
    emit anchorChanged(m_anchor);

    // Start drawing based on shape type
    m_isDrawing = true;

    switch (m_currentShapeType) {
    case Line:
        // First click: start point
        m_previewVertices.clear();
        m_previewVertices.push_back(pos3);
        // Second click will be handled in mouseReleaseEvent? Actually line uses two clicks.
        break;

    case Rectangle:
        // Click sets first corner
        m_startPoint = pos3;
        m_currentPoint = pos3;
        break;

    case Circle:
        // Click sets center
        m_startPoint = pos3;
        m_currentPoint = pos3;
        break;

    case Polygon:
        // Click adds a vertex
        if (m_previewVertices.empty()) {
            m_previewVertices.push_back(pos3);
        } else {
            // Add vertex only if we are in polygon mode and not finished
            // Double-click will finish polygon
            m_previewVertices.push_back(pos3);
        }
        break;
    }

    update();
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (!m_isDrawing) return;

    glm::vec2 worldPos = mapToWorld(event->pos());
    glm::vec3 pos3(worldPos.x, worldPos.y, 0.0f);

    switch (m_currentShapeType) {
    case Line:
        // Update the second point (temporary)
        if (m_previewVertices.size() == 1) {
            m_previewVertices.push_back(pos3);
        } else {
            m_previewVertices[1] = pos3;
        }
        break;

    case Rectangle:
        m_currentPoint = pos3;
        break;

    case Circle:
        m_currentPoint = pos3;
        break;

    case Polygon:
        // For polygon, we don't update preview on move; we only add on click
        break;
    }

    update();
}

void GLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton) return;
    if (!m_isDrawing) return;

    // For Line: second click finalizes
    if (m_currentShapeType == Line) {
        if (m_previewVertices.size() == 2) {
            // We have two points, finalize
            finalizeShape();
            m_isDrawing = false;
        }
        return;
    }

    // For Rectangle and Circle: release finalizes
    if (m_currentShapeType == Rectangle || m_currentShapeType == Circle) {
        finalizeShape();
        m_isDrawing = false;
        return;
    }

    // For Polygon: not finalized here (double-click or right-click)
}

void GLWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && m_currentShapeType == Polygon) {
        if (m_isDrawing && m_previewVertices.size() >= 2) {
            finalizeShape();
            m_isDrawing = false;
        }
    }
}

void GLWidget::updatePreviewLines()
{
    // Clear dynamic lines from previous frame
    m_lineRenderer.ClearDynamicLines();

    if (!m_isDrawing) return;

    QColor col = m_currentColor;
    glm::vec4 color(col.redF(), col.greenF(), col.blueF(), col.alphaF());
    float thickness = m_currentThickness;

    switch (m_currentShapeType) {
    case Line:
        if (m_previewVertices.size() >= 2) {
            m_lineRenderer.AddLine(m_previewVertices[0], m_previewVertices[1], color, thickness);
        }
        break;

    case Rectangle: {
        if (m_startPoint != m_currentPoint) {
            std::vector<glm::vec3> rect = {
                m_startPoint,
                {m_currentPoint.x, m_startPoint.y, 0.0f},
                m_currentPoint,
                {m_startPoint.x, m_currentPoint.y, 0.0f},
                m_startPoint // close
            };
            m_lineRenderer.AddLineStrip(rect, color, thickness);
        }
        break;
    }

    case Circle: {
        if (m_startPoint != m_currentPoint) {
            float radius = glm::distance(glm::vec2(m_startPoint), glm::vec2(m_currentPoint));
            const int segments = 36;
            std::vector<glm::vec3> points;
            for (int i = 0; i <= segments; ++i) {
                float angle = 2.0f * 3.14159265f * i / segments;
                float x = m_startPoint.x + radius * cos(angle);
                float y = m_startPoint.y + radius * sin(angle);
                points.emplace_back(x, y, 0.0f);
            }
            m_lineRenderer.AddLineStrip(points, color, thickness);
        }
        break;
    }

    case Polygon:
        if (m_previewVertices.size() >= 2) {
            // Draw the polygon as line strip (not closed until finalized)
            m_lineRenderer.AddLineStrip(m_previewVertices, color, thickness);
        }
        break;
    }
}

void GLWidget::finalizeShape()
{
    if (!m_isDrawing) return;

    ShapeData shape;
    shape.label = m_currentLabel;
    shape.color = m_currentColor;
    shape.thickness = m_currentThickness;
    shape.anchor = m_anchor;

    switch (m_currentShapeType) {
    case Line:
        shape.typeName = "Line";
        if (m_previewVertices.size() >= 2) {
            shape.vertices = m_previewVertices;
        }
        break;

    case Rectangle:
        shape.typeName = "Rectangle";
        if (m_startPoint != m_currentPoint) {
            shape.vertices = {
                m_startPoint,
                {m_currentPoint.x, m_startPoint.y, 0.0f},
                m_currentPoint,
                {m_startPoint.x, m_currentPoint.y, 0.0f},
                m_startPoint // close
            };
        }
        break;

    case Circle:
        shape.typeName = "Circle";
        if (m_startPoint != m_currentPoint) {
            float radius = glm::distance(glm::vec2(m_startPoint), glm::vec2(m_currentPoint));
            const int segments = 36;
            for (int i = 0; i <= segments; ++i) {
                float angle = 2.0f * 3.14159265f * i / segments;
                float x = m_startPoint.x + radius * cos(angle);
                float y = m_startPoint.y + radius * sin(angle);
                shape.vertices.emplace_back(x, y, 0.0f);
            }
        }
        break;

    case Polygon:
        shape.typeName = "Polygon";
        if (m_previewVertices.size() >= 2) {
            shape.vertices = m_previewVertices;
            // close polygon?
            // Usually polygon is closed, so we add first vertex at the end if not already closed
            if (!shape.vertices.empty() && shape.vertices.front() != shape.vertices.back()) {
                shape.vertices.push_back(shape.vertices.front());
            }
        }
        break;
    }

    if (shape.vertices.size() >= 2) {
        // Add to static lines and store shape
        addShapeToRenderer(shape);
        m_shapes.push_back(shape);
        // Emit signal to update table
        emit shapeAdded(shape.label, shape.typeName, shape.color, shape.thickness, shape.anchor);
    }

    // Reset drawing state
    m_previewVertices.clear();
    m_isDrawing = false;
    update();
}

void GLWidget::addShapeToRenderer(const ShapeData& shape)
{
    glm::vec4 color(shape.color.redF(), shape.color.greenF(), shape.color.blueF(), shape.color.alphaF());
    float thickness = shape.thickness;
    m_lineRenderer.AddStaticLineStrip(shape.vertices, color, thickness);
}

void GLWidget::addShape()
{
    // If currently drawing, finalize it first
    if (m_isDrawing) {
        finalizeShape();
    }
    // Nothing else to add; the shape is added when finalized.
    // But we could also allow adding a shape defined solely by anchor and parameters without drawing.
    // For simplicity, we rely on drawing.
}

void GLWidget::clearAllShapes()
{
    m_lineRenderer.ClearStaticLines();
    m_shapes.clear();
    emit shapesCleared();
    update();
}