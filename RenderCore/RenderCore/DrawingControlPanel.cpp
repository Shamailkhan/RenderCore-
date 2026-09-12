#include "DrawingControlPanel.h"
#include "GeoUtils.h"

#include <QComboBox>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QLabel>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QColorDialog>
#include <QHeaderView>
#include <QDoubleValidator>
#include <QMessageBox>


// ============================================================
// CONSTRUCTOR
// ============================================================

DrawingControlPanel::DrawingControlPanel(QWidget* parent)
    : QWidget(parent)
{
    BuildUi();
    UpdateColorSwatch();
    UpdateFieldVisibilityForType();
}


// ============================================================
// UI CONSTRUCTION
// ============================================================

void DrawingControlPanel::BuildUi()
{
    auto* mainLayout = new QVBoxLayout(this);

    // ------------------------------------------------------------
    // Tool settings group
    // ------------------------------------------------------------
    auto* toolGroup = new QGroupBox(tr("Shape Settings"), this);
    auto* toolForm = new QFormLayout(toolGroup);

    m_shapeTypeCombo = new QComboBox(toolGroup);
    m_shapeTypeCombo->addItem(tr("Line"),      QVariant::fromValue(static_cast<int>(ShapeType::Line)));
    m_shapeTypeCombo->addItem(tr("Polygon"),   QVariant::fromValue(static_cast<int>(ShapeType::Polygon)));
    m_shapeTypeCombo->addItem(tr("Rectangle"), QVariant::fromValue(static_cast<int>(ShapeType::Rectangle)));
    m_shapeTypeCombo->addItem(tr("Circle"),    QVariant::fromValue(static_cast<int>(ShapeType::Circle)));
    toolForm->addRow(tr("Shape type:"), m_shapeTypeCombo);

    m_colorButton = new QPushButton(tr("Choose Color..."), toolGroup);
    toolForm->addRow(tr("Color:"), m_colorButton);

    m_thicknessSpin = new QDoubleSpinBox(toolGroup);
    m_thicknessSpin->setRange(0.1, 100.0);
    m_thicknessSpin->setSingleStep(0.5);
    m_thicknessSpin->setValue(3.0);
    m_thicknessSpin->setSuffix(tr(" px"));
    toolForm->addRow(tr("Line intensity\n(thickness):"), m_thicknessSpin);

    m_labelEdit = new QLineEdit(toolGroup);
    m_labelEdit->setPlaceholderText(tr("e.g. Fence line, Zone A..."));
    toolForm->addRow(tr("Label:"), m_labelEdit);

    mainLayout->addWidget(toolGroup);

    // ------------------------------------------------------------
    // Lat/long entry group
    // ------------------------------------------------------------
    auto* geoGroup = new QGroupBox(tr("Add by Latitude / Longitude"), this);
    auto* geoLayout = new QVBoxLayout(geoGroup);

    auto* latLonForm = new QFormLayout();

    m_latEdit = new QLineEdit(geoGroup);
    m_latEdit->setPlaceholderText(tr("e.g. 33.5651"));
    m_latEdit->setValidator(new QDoubleValidator(-90.0, 90.0, 8, m_latEdit));
    latLonForm->addRow(tr("Latitude:"), m_latEdit);

    m_lonEdit = new QLineEdit(geoGroup);
    m_lonEdit->setPlaceholderText(tr("e.g. 73.0169"));
    m_lonEdit->setValidator(new QDoubleValidator(-180.0, 180.0, 8, m_lonEdit));
    latLonForm->addRow(tr("Longitude:"), m_lonEdit);

    geoLayout->addLayout(latLonForm);

    m_addPointButton = new QPushButton(tr("Add Point to Shape"), geoGroup);
    geoLayout->addWidget(m_addPointButton);

    m_pendingPointsList = new QListWidget(geoGroup);
    m_pendingPointsList->setMaximumHeight(90);
    geoLayout->addWidget(m_pendingPointsList);

    m_removePendingPointButton = new QPushButton(tr("Remove Selected Point"), geoGroup);
    geoLayout->addWidget(m_removePendingPointButton);

    auto* radiusForm = new QFormLayout();
    m_radiusLabel = new QLabel(tr("Radius (km):"), geoGroup);
    m_radiusSpin = new QDoubleSpinBox(geoGroup);
    m_radiusSpin->setRange(0.001, 100000.0);
    m_radiusSpin->setDecimals(3);
    m_radiusSpin->setValue(1.0);
    radiusForm->addRow(m_radiusLabel, m_radiusSpin);
    geoLayout->addLayout(radiusForm);

    m_addShapeButton = new QPushButton(tr("Add Shape"), geoGroup);
    m_addShapeButton->setStyleSheet(QStringLiteral("font-weight: bold;"));
    geoLayout->addWidget(m_addShapeButton);

    auto* geoHint = new QLabel(
        tr("Line/Rectangle need 2 points, Polygon needs 2+, Circle needs "
           "1 point (center) plus a radius."),
        geoGroup
    );
    geoHint->setWordWrap(true);
    geoHint->setStyleSheet(QStringLiteral("color: gray; font-size: 11px;"));
    geoLayout->addWidget(geoHint);

    mainLayout->addWidget(geoGroup);

    // ------------------------------------------------------------
    // Shapes table
    // ------------------------------------------------------------
    auto* tableGroup = new QGroupBox(tr("Shapes Added"), this);
    auto* tableLayout = new QVBoxLayout(tableGroup);

    m_table = new QTableWidget(0, 6, tableGroup);
    m_table->setHorizontalHeaderLabels({
        tr("#"), tr("Label"), tr("Type"), tr("Color"), tr("Thickness"), tr("Points")
    });
    m_table->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableLayout->addWidget(m_table);

    auto* tableButtons = new QHBoxLayout();
    m_removeSelectedButton = new QPushButton(tr("Remove Selected"), tableGroup);
    m_clearAllButton = new QPushButton(tr("Clear All"), tableGroup);
    tableButtons->addWidget(m_removeSelectedButton);
    tableButtons->addWidget(m_clearAllButton);
    tableLayout->addLayout(tableButtons);

    mainLayout->addWidget(tableGroup, 1);

    // ------------------------------------------------------------
    // Status label
    // ------------------------------------------------------------
    m_statusLabel = new QLabel(tr("Ready."), this);
    m_statusLabel->setWordWrap(true);
    m_statusLabel->setStyleSheet(QStringLiteral("color: gray;"));
    mainLayout->addWidget(m_statusLabel);

    setLayout(mainLayout);
    setMinimumWidth(340);

    // ------------------------------------------------------------
    // Signal wiring
    // ------------------------------------------------------------
    connect(m_shapeTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &DrawingControlPanel::OnShapeTypeComboChanged);

    connect(m_colorButton, &QPushButton::clicked,
            this, &DrawingControlPanel::OnPickColorClicked);

    connect(m_thicknessSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [this](double value) { emit thicknessChanged(static_cast<float>(value)); });

    connect(m_labelEdit, &QLineEdit::textChanged,
            this, &DrawingControlPanel::labelChanged);

    connect(m_addPointButton, &QPushButton::clicked,
            this, &DrawingControlPanel::OnAddPointClicked);

    connect(m_removePendingPointButton, &QPushButton::clicked,
            this, &DrawingControlPanel::OnRemovePendingPointClicked);

    connect(m_addShapeButton, &QPushButton::clicked,
            this, &DrawingControlPanel::OnAddShapeClicked);

    connect(m_removeSelectedButton, &QPushButton::clicked,
            this, &DrawingControlPanel::OnRemoveSelectedClicked);

    connect(m_clearAllButton, &QPushButton::clicked,
            this, &DrawingControlPanel::OnClearAllClicked);
}


// ============================================================
// SHAPE TYPE
// ============================================================

ShapeType DrawingControlPanel::CurrentComboShapeType() const
{
    return static_cast<ShapeType>(m_shapeTypeCombo->currentData().toInt());
}

void DrawingControlPanel::OnShapeTypeComboChanged(int /*comboIndex*/)
{
    const ShapeType type = CurrentComboShapeType();
    UpdateFieldVisibilityForType();
    emit shapeTypeChanged(type);
}

void DrawingControlPanel::UpdateFieldVisibilityForType()
{
    const ShapeType type = CurrentComboShapeType();
    const bool isCircle = (type == ShapeType::Circle);

    m_radiusLabel->setVisible(isCircle);
    m_radiusSpin->setVisible(isCircle);
}


// ============================================================
// COLOR
// ============================================================

void DrawingControlPanel::OnPickColorClicked()
{
    const QColor picked = QColorDialog::getColor(
        m_currentColor, this, tr("Choose Line Color"),
        QColorDialog::ShowAlphaChannel
    );

    if (!picked.isValid())
        return;

    m_currentColor = picked;
    UpdateColorSwatch();

    emit colorChanged(m_currentColor);
}

void DrawingControlPanel::UpdateColorSwatch()
{
    m_colorButton->setStyleSheet(QStringLiteral(
        "background-color: %1; color: %2;"
    ).arg(
        m_currentColor.name(QColor::HexArgb),
        m_currentColor.lightnessF() > 0.5 ? "black" : "white"
    ));
    m_colorButton->setText(m_currentColor.name(QColor::HexRgb));
}


// ============================================================
// LAT/LONG PENDING POINTS
// ============================================================

void DrawingControlPanel::OnAddPointClicked()
{
    bool latOk = false;
    bool lonOk = false;

    const double lat = m_latEdit->text().toDouble(&latOk);
    const double lon = m_lonEdit->text().toDouble(&lonOk);

    if (!latOk || !lonOk)
    {
        QMessageBox::warning(this, tr("Invalid coordinate"),
            tr("Please enter a valid latitude and longitude."));
        return;
    }

    m_pendingGeoPoints.push_back({ lat, lon });

    m_pendingPointsList->addItem(
        tr("Point %1: %2, %3")
            .arg(m_pendingGeoPoints.size())
            .arg(lat, 0, 'f', 6)
            .arg(lon, 0, 'f', 6)
    );

    m_latEdit->clear();
    m_lonEdit->clear();
    m_latEdit->setFocus();
}

void DrawingControlPanel::OnRemovePendingPointClicked()
{
    const int row = m_pendingPointsList->currentRow();
    if (row < 0 || row >= static_cast<int>(m_pendingGeoPoints.size()))
        return;

    m_pendingGeoPoints.erase(m_pendingGeoPoints.begin() + row);
    delete m_pendingPointsList->takeItem(row);
}


// ============================================================
// ADD SHAPE (from lat/long)
// ============================================================

void DrawingControlPanel::OnAddShapeClicked()
{
    const ShapeType type = CurrentComboShapeType();

    const int minPoints =
        (type == ShapeType::Circle) ? 1 : 2;

    if (static_cast<int>(m_pendingGeoPoints.size()) < minPoints)
    {
        QMessageBox::warning(this, tr("Not enough points"),
            tr("This shape needs at least %1 point(s). "
               "Use \"Add Point to Shape\" to add lat/long coordinates first.")
               .arg(minPoints));
        return;
    }

    Shape shape;
    shape.type = type;
    shape.label = m_labelEdit->text();
    shape.color = m_currentColor;
    shape.thickness = static_cast<float>(m_thicknessSpin->value());
    shape.hasGeo = true;
    shape.geoPoints = m_pendingGeoPoints;

    for (const GeoPoint& gp : m_pendingGeoPoints)
        shape.points.push_back(GeoUtils::LatLonToWorld(gp));

    if (type == ShapeType::Circle)
        shape.radius = static_cast<float>(m_radiusSpin->value());

    if (type == ShapeType::Rectangle && shape.points.size() > 2)
        shape.points.resize(2); // rectangle only uses the first two corners

    emit addShapeFromGeoRequested(shape);

    // Reset the pending-points list for the next shape.
    m_pendingGeoPoints.clear();
    m_pendingPointsList->clear();
}


// ============================================================
// TABLE
// ============================================================

void DrawingControlPanel::OnShapeAdded(const Shape& shape, int index)
{
    const int row = m_table->rowCount();
    m_table->insertRow(row);

    m_table->setItem(row, 0, new QTableWidgetItem(QString::number(index)));
    m_table->setItem(row, 1, new QTableWidgetItem(
        shape.label.isEmpty() ? tr("(unlabeled)") : shape.label));
    m_table->setItem(row, 2, new QTableWidgetItem(ShapeTypeToString(shape.type)));

    auto* colorItem = new QTableWidgetItem(shape.color.name(QColor::HexRgb));
    colorItem->setBackground(shape.color);
    colorItem->setForeground(shape.color.lightnessF() > 0.5 ? Qt::black : Qt::white);
    m_table->setItem(row, 3, colorItem);

    m_table->setItem(row, 4, new QTableWidgetItem(
        QString::number(shape.thickness, 'f', 1)));

    QString pointsSummary;
    if (shape.type == ShapeType::Circle)
    {
        pointsSummary = tr("center=(%1, %2), r=%3")
            .arg(shape.points.empty() ? 0.0f : shape.points[0].x, 0, 'f', 2)
            .arg(shape.points.empty() ? 0.0f : shape.points[0].y, 0, 'f', 2)
            .arg(shape.radius, 0, 'f', 2);
    }
    else
    {
        QStringList parts;
        for (const auto& p : shape.points)
            parts << QStringLiteral("(%1, %2)").arg(p.x, 0, 'f', 2).arg(p.y, 0, 'f', 2);
        pointsSummary = parts.join(QStringLiteral(" -> "));
    }
    m_table->setItem(row, 5, new QTableWidgetItem(pointsSummary));

    m_table->scrollToBottom();
}

void DrawingControlPanel::RefreshTable(const std::vector<Shape>& shapes)
{
    m_table->setRowCount(0);

    for (size_t i = 0; i < shapes.size(); ++i)
        OnShapeAdded(shapes[i], static_cast<int>(i));
}

void DrawingControlPanel::OnRemoveSelectedClicked()
{
    const int row = m_table->currentRow();
    if (row < 0)
        return;

    emit removeShapeRequested(row);
}

void DrawingControlPanel::OnClearAllClicked()
{
    emit clearAllRequested();
}


// ============================================================
// STATUS
// ============================================================

void DrawingControlPanel::SetStatusMessage(const QString& text)
{
    m_statusLabel->setText(text);
}
