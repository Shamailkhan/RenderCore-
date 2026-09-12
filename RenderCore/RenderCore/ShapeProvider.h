#pragma once

// GLEW must be the first GL-related header seen by this translation
// unit - see the note in GLCanvas.h for why.
#include <GL/glew.h>

#include <QObject>
#include <QColor>
#include <QPoint>

#include <vector>

#include "Camera.h"
#include "ShaderManager.h"
#include "LineRenderer.h"
#include "Shape.h"

class QMouseEvent;
class QWheelEvent;
class QKeyEvent;

// ============================================================
// SHAPE PROVIDER
// ============================================================
//
// Owns everything about "what is being drawn and how": the
// Camera, ShaderManager and LineRenderer, the current tool
// settings (shape type/color/thickness/label), the in-progress
// draw state machine, and the finalized shape list.
//
// GLCanvas itself only contains the Qt/OpenGL event overrides
// (initializeGL/resizeGL/paintGL, mouse/wheel/key events) and
// forwards each one straight into this class. This class knows
// nothing about QOpenGLWidget - it just needs a current GL
// context when Initialize()/Resize()/Paint() are called, which
// GLCanvas guarantees by construction (Qt only calls those
// during the corresponding GL callbacks).
//
class ShapeProvider : public QObject
{
    Q_OBJECT

public:
    explicit ShapeProvider(QObject* parent = nullptr);
    ~ShapeProvider() override;

    // ------------------------------------------------------------
    // GL lifecycle - called from GLCanvas's matching overrides
    // ------------------------------------------------------------
    void Initialize();                 // from initializeGL()
    void Resize(int w, int h);         // from resizeGL()
    void Paint();                      // from paintGL()

    // ------------------------------------------------------------
    // Input - called from GLCanvas's matching overrides
    // ------------------------------------------------------------
    void HandleMousePress(QMouseEvent* event);
    void HandleMouseMove(QMouseEvent* event);
    void HandleMouseDoubleClick(QMouseEvent* event);
    void HandleWheel(QWheelEvent* event);
    void HandleKeyPress(QKeyEvent* event);

    // ------------------------------------------------------------
    // Shape list (used by the lat/long entry path too)
    // ------------------------------------------------------------
    const std::vector<Shape>& GetShapes() const { return m_shapes; }

public slots:
    // ------------------------------------------------------------
    // Tool configuration (bound directly to the control panel)
    // ------------------------------------------------------------
    void SetShapeType(ShapeType type);
    void SetColor(const QColor& color);
    void SetThickness(float thickness);
    void SetLabel(const QString& label);

    void CancelActiveDraw();

    int AddFinalizedShape(Shape shape);
    void RemoveShapeAt(int index);
    void ClearAllShapes();

signals:
    void shapeAdded(const Shape& shape, int index);
    void shapesReset();
    void statusMessage(const QString& text);
    void repaintNeeded(); // GLCanvas connects this to update()

private:
    void FinalizePolygonIfPossible();
    void RebuildStaticBufferFromShapes();
    void PushShapeIntoRenderer(const Shape& shape);
    void UpdateStatusMessage();

    // The single place that turns a Shape's type + points into the
    // vertex loop LineRenderer actually draws. Used both when
    // iterating m_shapes to (re)build the static buffer, and for the
    // live preview in Paint() (built from a temporary Shape
    // representing whatever is currently being drawn) - so there is
    // only one type switch in the whole class, not one per call site.
    std::vector<glm::vec3> BuildShapeVertices(const Shape& shape, bool closeLoop) const;

    // Builds the temporary Shape representing whatever is currently
    // being drawn (from m_currentType/m_activePoints/m_previewPoint),
    // so Paint() can run it through BuildShapeVertices() exactly like
    // any entry in m_shapes.
    Shape BuildPreviewShape() const;

    glm::vec3 WidgetPosToWorld(const QPoint& pos) const;
    glm::vec4 CurrentColorVec() const;

    static std::vector<glm::vec3> BuildRectangleLoop(
        const glm::vec3& cornerA,
        const glm::vec3& cornerB);

    static std::vector<glm::vec3> BuildCircleLoop(
        const glm::vec3& center,
        float radius,
        int segments = 64);

    // ------------------------------------------------------------
    // Rendering
    // ------------------------------------------------------------
    Camera         m_camera;
    ShaderManager  m_shaderManager;
    LineRenderer   m_lineRenderer;

    // ------------------------------------------------------------
    // Shape list
    // ------------------------------------------------------------
    std::vector<Shape> m_shapes;

    // ------------------------------------------------------------
    // Current tool state
    // ------------------------------------------------------------
    ShapeType m_currentType = ShapeType::Line;
    QColor    m_currentColor = QColor(255, 60, 60);
    float     m_currentThickness = 3.0f;
    QString   m_currentLabel;

    // ------------------------------------------------------------
    // In-progress draw state
    // ------------------------------------------------------------
    bool                    m_isDrawing = false;
    std::vector<glm::vec3>  m_activePoints;
    glm::vec3               m_previewPoint{0.0f, 0.0f, 0.0f};

    // ------------------------------------------------------------
    // Camera panning (middle mouse drag)
    // ------------------------------------------------------------
    bool   m_isPanning = false;
    QPoint m_lastPanPos;
};
