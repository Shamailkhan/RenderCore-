#pragma once

#include <QWidget>

#include "Shape.h"

class QComboBox;
class QPushButton;
class QDoubleSpinBox;
class QLineEdit;
class QLabel;
class QTableWidget;
class QListWidget;

// ============================================================
// DRAWING CONTROL PANEL
// ============================================================
//
// The side panel described by the user:
//   - shape type combo (Line / Polygon / Rectangle / Circle)
//   - color field
//   - line intensity (thickness) field
//   - label field
//   - lat/long line edits + "Add Point"/"Add Shape" buttons, for
//     defining a shape purely from coordinates instead of the mouse
//   - a table listing every shape that has been added
//
// This widget does not know about OpenGL at all - it only emits
// signals describing what the user configured, and MainWindow wires
// those into the GLCanvas.
//
class DrawingControlPanel : public QWidget
{
    Q_OBJECT

public:
    explicit DrawingControlPanel(QWidget* parent = nullptr);

public slots:
    // Called by MainWindow whenever a shape is added (from either
    // the canvas or this panel's own "Add Shape" button) so the
    // table stays in sync.
    void OnShapeAdded(const Shape& shape, int index);

    // Called when the shape list is rebuilt wholesale (remove/clear).
    void RefreshTable(const std::vector<Shape>& shapes);

    void SetStatusMessage(const QString& text);

signals:
    void shapeTypeChanged(ShapeType type);
    void colorChanged(const QColor& color);
    void thicknessChanged(float thickness);
    void labelChanged(const QString& label);

    // User filled in lat/long fields (+ radius for circles) and
    // pressed "Add Shape".
    void addShapeFromGeoRequested(Shape shape);

    void removeShapeRequested(int index);
    void clearAllRequested();

private slots:
    void OnShapeTypeComboChanged(int comboIndex);
    void OnPickColorClicked();
    void OnAddPointClicked();
    void OnRemovePendingPointClicked();
    void OnAddShapeClicked();
    void OnRemoveSelectedClicked();
    void OnClearAllClicked();

private:
    void BuildUi();
    void UpdateColorSwatch();
    void UpdateFieldVisibilityForType();
    ShapeType CurrentComboShapeType() const;

    QComboBox*      m_shapeTypeCombo = nullptr;
    QPushButton*    m_colorButton = nullptr;
    QDoubleSpinBox* m_thicknessSpin = nullptr;
    QLineEdit*      m_labelEdit = nullptr;

    QLineEdit*      m_latEdit = nullptr;
    QLineEdit*      m_lonEdit = nullptr;
    QPushButton*    m_addPointButton = nullptr;
    QListWidget*    m_pendingPointsList = nullptr;
    QPushButton*    m_removePendingPointButton = nullptr;

    QLabel*         m_radiusLabel = nullptr;
    QDoubleSpinBox* m_radiusSpin = nullptr; // kilometers, Circle only

    QPushButton*    m_addShapeButton = nullptr;

    QTableWidget*   m_table = nullptr;
    QPushButton*    m_removeSelectedButton = nullptr;
    QPushButton*    m_clearAllButton = nullptr;

    QLabel*         m_statusLabel = nullptr;

    QColor m_currentColor = QColor(255, 60, 60);
    std::vector<GeoPoint> m_pendingGeoPoints;
};
