#include "GLCanvas.h"

#include <QSurfaceFormat>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>


// ============================================================
// CONSTRUCTOR / DESTRUCTOR
// ============================================================

GLCanvas::GLCanvas(QWidget* parent)
    : QOpenGLWidget(parent)
    , m_shapeProvider(this)
{
    QSurfaceFormat format;
    format.setVersion(4, 5);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);
    setFormat(format);

    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    // The shape provider cannot call update() on itself (it isn't a widget),
    // so it asks for a repaint through this signal instead.
    connect(&m_shapeProvider, &ShapeProvider::repaintNeeded, this, [this]() { update(); });
}

GLCanvas::~GLCanvas()
{
    // m_shapeProvider (and its LineRenderer's GL buffers) is destroyed right
    // after this body returns, so the context needs to still be
    // current at that point - hence no matching doneCurrent() here.
    makeCurrent();
}


// ============================================================
// GL LIFECYCLE - each one just forwards to the shape provider
// ============================================================

void GLCanvas::initializeGL()
{
    m_shapeProvider.Initialize();
}

void GLCanvas::resizeGL(int w, int h)
{
    m_shapeProvider.Resize(w, h);
}

void GLCanvas::paintGL()
{
    m_shapeProvider.Paint();
}


// ============================================================
// INPUT EVENTS - each one just forwards to the shape provider
// ============================================================

void GLCanvas::mousePressEvent(QMouseEvent* event)
{
    m_shapeProvider.HandleMousePress(event);
}

void GLCanvas::mouseMoveEvent(QMouseEvent* event)
{
    m_shapeProvider.HandleMouseMove(event);
}

void GLCanvas::mouseDoubleClickEvent(QMouseEvent* event)
{
    m_shapeProvider.HandleMouseDoubleClick(event);
}

void GLCanvas::wheelEvent(QWheelEvent* event)
{
    m_shapeProvider.HandleWheel(event);
}

void GLCanvas::keyPressEvent(QKeyEvent* event)
{
    m_shapeProvider.HandleKeyPress(event);
}
