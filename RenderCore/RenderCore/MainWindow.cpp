#include "MainWindow.h"
#include "GLCanvas.h"
#include "ShapeProvider.h"
#include "DrawingControlPanel.h"

#include <QSplitter>
#include <QStatusBar>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Line/Shape Drawing Tool"));
    resize(1400, 900);

    auto* splitter = new QSplitter(Qt::Horizontal, this);

    m_canvas = new GLCanvas(splitter);
    m_panel  = new DrawingControlPanel(splitter);

    splitter->addWidget(m_canvas);
    splitter->addWidget(m_panel);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 0);

    setCentralWidget(splitter);
    statusBar()->showMessage(tr("Ready."));

    // ------------------------------------------------------------
    // Panel -> Engine (tool configuration)
    // ------------------------------------------------------------
    ShapeProvider& shapeProvider = m_canvas->GetShapeProvider();

    connect(m_panel, &DrawingControlPanel::shapeTypeChanged,
            &shapeProvider, &ShapeProvider::SetShapeType);

    connect(m_panel, &DrawingControlPanel::colorChanged,
            &shapeProvider, &ShapeProvider::SetColor);

    connect(m_panel, &DrawingControlPanel::thicknessChanged,
            &shapeProvider, &ShapeProvider::SetThickness);

    connect(m_panel, &DrawingControlPanel::labelChanged,
            &shapeProvider, &ShapeProvider::SetLabel);

    // ------------------------------------------------------------
    // Panel -> Engine (lat/long "Add Shape" path)
    // ------------------------------------------------------------
    connect(m_panel, &DrawingControlPanel::addShapeFromGeoRequested,
            &shapeProvider, [&shapeProvider](Shape shape) { shapeProvider.AddFinalizedShape(std::move(shape)); });

    connect(m_panel, &DrawingControlPanel::removeShapeRequested,
            &shapeProvider, &ShapeProvider::RemoveShapeAt);

    connect(m_panel, &DrawingControlPanel::clearAllRequested,
            &shapeProvider, &ShapeProvider::ClearAllShapes);

    // ------------------------------------------------------------
    // Engine -> Panel (keep table + status in sync)
    // ------------------------------------------------------------
    connect(&shapeProvider, &ShapeProvider::shapeAdded,
            m_panel, &DrawingControlPanel::OnShapeAdded);

    connect(&shapeProvider, &ShapeProvider::shapesReset,
            m_panel, [this, &shapeProvider]() { m_panel->RefreshTable(shapeProvider.GetShapes()); });

    connect(&shapeProvider, &ShapeProvider::statusMessage,
            m_panel, &DrawingControlPanel::SetStatusMessage);

    connect(&shapeProvider, &ShapeProvider::statusMessage,
            statusBar(), [this](const QString& text) { statusBar()->showMessage(text); });
}
