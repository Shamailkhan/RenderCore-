#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QVector>
#include <QColor>
#include <glm/glm.hpp>
#include "LineRenderer.h"
#include "Camera.h"
#include "Shader.h"

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    enum ShapeType { Line, Polygon, Rectangle, Circle };
    GLWidget(QWidget* parent = nullptr);
    ~GLWidget();

    void setShapeType(ShapeType type);
    void setColor(const QColor& color);
    void setThickness(float thickness);
    void setLabel(const QString& label);
    void setAnchor(const glm::vec2& anchor); // latitude/longitude
    glm::vec2 getAnchor() const;

public slots:
    void addShape();          // Add current preview to static lines and table
    void clearAllShapes();    // Remove all static shapes

signals:
    void anchorChanged(const glm::vec2& pos); // emitted when user clicks on widget
    void shapeAdded(const QString& label, const QString& type, const QColor& color, float thickness, const glm::vec2& anchor);
    void shapesCleared();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
    struct ShapeData {
        QString label;
        QString typeName;
        QColor color;
        float thickness;
        glm::vec2 anchor;   // position (lat/long)
        std::vector<glm::vec3> vertices; // world coordinates
    };

    // Drawing state
    ShapeType m_currentShapeType;
    QColor m_currentColor;
    float m_currentThickness;
    QString m_currentLabel;
    glm::vec2 m_anchor;              // current anchor (lat/long)

    // Preview shape being drawn
    bool m_isDrawing;
    std::vector<glm::vec3> m_previewVertices;  // for lines/polygon
    glm::vec3 m_startPoint;           // for rectangle/circle
    glm::vec3 m_currentPoint;         // for rectangle/circle

    // Added shapes
    QVector<ShapeData> m_shapes;

    // OpenGL objects
    LineRenderer m_lineRenderer;
    Shader m_shader;
    Camera m_camera;

    // Helpers
    glm::vec2 mapToWorld(const QPoint& pos) const;
    void updatePreviewLines();
    void finalizeShape();
    void addShapeToRenderer(const ShapeData& shape);
};