#pragma once

// GLEW must be the first GL-related header seen by this translation
// unit - Qt's own OpenGL headers (pulled in via QOpenGLWidget below)
// include <GL/gl.h>, and GLEW needs to come first or its function
// pointer typedefs clash with the system GL headers.
#include <GL/glew.h>

#include <QOpenGLWidget>

#include "ShapeProvider.h"

// ============================================================
// GLCANVAS
// ============================================================
//
// Thin Qt/OpenGL shell only: GL lifecycle callbacks
// (initializeGL/resizeGL/paintGL) and raw input event overrides
// (mouse/wheel/key). Every one of them just forwards to
// ShapeProvider, which owns the camera, shaders, renderer, tool
// state, and the click-to-draw state machine.
//
// MainWindow talks to the shape provider directly via
// GetShapeProvider() - it connects DrawingControlPanel's
// signals/slots straight to it, not through this class.
//
class GLCanvas : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit GLCanvas(QWidget* parent = nullptr);
    ~GLCanvas() override;

    ShapeProvider& GetShapeProvider() { return m_shapeProvider; }

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    ShapeProvider m_shapeProvider;
};
