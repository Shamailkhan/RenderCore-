#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QColorDialog>
#include <QLabel>
#include <QHeaderView>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_currentColor(Qt::white)
{
    setWindowTitle("Line Drawing App");
    resize(1000, 700);

    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);

    QHBoxLayout* mainLayout = new QHBoxLayout(m_centralWidget);

    // Left: OpenGL widget
    m_glWidget = new GLWidget(this);
    mainLayout->addWidget(m_glWidget, 2);

    // Right: Control panel
    QVBoxLayout* controlLayout = new QVBoxLayout();
    mainLayout->addLayout(controlLayout, 1);

    // Shape type
    QGroupBox* shapeGroup = new QGroupBox("Shape");
    QVBoxLayout* shapeLayout = new QVBoxLayout(shapeGroup);
    m_shapeCombo = new QComboBox();
    m_shapeCombo->addItem("Line");
    m_shapeCombo->addItem("Polygon");
    m_shapeCombo->addItem("Rectangle");
    m_shapeCombo->addItem("Circle");
    connect(m_shapeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onShapeTypeChanged);
    shapeLayout->addWidget(m_shapeCombo);
    controlLayout->addWidget(shapeGroup);

    // Color
    QGroupBox* colorGroup = new QGroupBox("Color");
    QVBoxLayout* colorLayout = new QVBoxLayout(colorGroup);
    m_colorButton = new QPushButton();
    m_colorButton->setStyleSheet("background-color: white;");
    connect(m_colorButton, &QPushButton::clicked, this, &MainWindow::onColorButtonClicked);
    colorLayout->addWidget(m_colorButton);
    controlLayout->addWidget(colorGroup);

    // Thickness
    QGroupBox* thickGroup = new QGroupBox("Thickness");
    QVBoxLayout* thickLayout = new QVBoxLayout(thickGroup);
    m_thicknessSpin = new QDoubleSpinBox();
    m_thicknessSpin->setRange(0.1, 20.0);
    m_thicknessSpin->setSingleStep(0.5);
    m_thicknessSpin->setValue(2.0);
    thickLayout->addWidget(m_thicknessSpin);
    controlLayout->addWidget(thickGroup);

    // Label
    QGroupBox* labelGroup = new QGroupBox("Label");
    QVBoxLayout* labelLayout = new QVBoxLayout(labelGroup);
    m_labelEdit = new QLineEdit();
    m_labelEdit->setPlaceholderText("Shape label");
    labelLayout->addWidget(m_labelEdit);
    controlLayout->addWidget(labelGroup);

    // Latitude / Longitude
    QGroupBox* latlonGroup = new QGroupBox("Position (Lat/Long)");
    QGridLayout* latlonLayout = new QGridLayout(latlonGroup);
    latlonLayout->addWidget(new QLabel("Lat:"), 0, 0);
    m_latEdit = new QLineEdit("0.0");
    latlonLayout->addWidget(m_latEdit, 0, 1);
    latlonLayout->addWidget(new QLabel("Long:"), 1, 0);
    m_lonEdit = new QLineEdit("0.0");
    latlonLayout->addWidget(m_lonEdit, 1, 1);
    controlLayout->addWidget(latlonGroup);

    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    m_addButton = new QPushButton("Add");
    connect(m_addButton, &QPushButton::clicked, this, &MainWindow::onAddButtonClicked);
    buttonLayout->addWidget(m_addButton);

    m_clearButton = new QPushButton("Clear All");
    connect(m_clearButton, &QPushButton::clicked, this, &MainWindow::onClearButtonClicked);
    buttonLayout->addWidget(m_clearButton);
    controlLayout->addLayout(buttonLayout);

    // Table
    QGroupBox* tableGroup = new QGroupBox("Added Shapes");
    QVBoxLayout* tableLayout = new QVBoxLayout(tableGroup);
    m_tableWidget = new QTableWidget(0, 6);
    m_tableWidget->setHorizontalHeaderLabels({"Label", "Type", "Color", "Thickness", "Lat", "Long"});
    m_tableWidget->horizontalHeader()->setStretchLastSection(true);
    tableLayout->addWidget(m_tableWidget);
    controlLayout->addWidget(tableGroup);

    // Connect signals from GLWidget
    connect(m_glWidget, &GLWidget::anchorChanged, this, &MainWindow::onAnchorChanged);
    connect(m_glWidget, &GLWidget::shapeAdded, this, &MainWindow::onShapeAdded);
    connect(m_glWidget, &GLWidget::shapesCleared, this, &MainWindow::onShapesCleared);

    // Set initial anchor from lat/lon fields
    float lat = m_latEdit->text().toFloat();
    float lon = m_lonEdit->text().toFloat();
    m_glWidget->setAnchor(glm::vec2(lat, lon));
}

MainWindow::~MainWindow() {}

void MainWindow::onShapeTypeChanged(int index)
{
    GLWidget::ShapeType type = static_cast<GLWidget::ShapeType>(index);
    m_glWidget->setShapeType(type);
}

void MainWindow::onColorButtonClicked()
{
    QColor color = QColorDialog::getColor(m_currentColor, this, "Select Color");
    if (color.isValid()) {
        m_currentColor = color;
        m_colorButton->setStyleSheet(QString("background-color: %1;").arg(color.name()));
        m_glWidget->setColor(color);
    }
}

void MainWindow::onAddButtonClicked()
{
    // Update anchor from lat/lon
    float lat = m_latEdit->text().toFloat();
    float lon = m_lonEdit->text().toFloat();
    m_glWidget->setAnchor(glm::vec2(lat, lon));

    // Update label and thickness
    m_glWidget->setLabel(m_labelEdit->text());
    m_glWidget->setThickness((float)m_thicknessSpin->value());

    // Tell GLWidget to finalize current shape and add it
    m_glWidget->addShape();
}

void MainWindow::onClearButtonClicked()
{
    m_glWidget->clearAllShapes();
}

void MainWindow::onAnchorChanged(const glm::vec2& pos)
{
    m_latEdit->setText(QString::number(pos.x, 'f', 2));
    m_lonEdit->setText(QString::number(pos.y, 'f', 2));
}

void MainWindow::onShapeAdded(const QString& label, const QString& type,
                              const QColor& color, float thickness,
                              const glm::vec2& anchor)
{
    int row = m_tableWidget->rowCount();
    m_tableWidget->insertRow(row);
    m_tableWidget->setItem(row, 0, new QTableWidgetItem(label));
    m_tableWidget->setItem(row, 1, new QTableWidgetItem(type));
    QPixmap pix(16, 16);
    pix.fill(color);
    m_tableWidget->setItem(row, 2, new QTableWidgetItem(QIcon(pix), ""));
    m_tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(thickness)));
    m_tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(anchor.x, 'f', 2)));
    m_tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(anchor.y, 'f', 2)));
}

void MainWindow::onShapesCleared()
{
    m_tableWidget->setRowCount(0);
}