#pragma once

#include <QMainWindow>
#include <QTableWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QColor>
#include "GLWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onShapeTypeChanged(int index);
    void onColorButtonClicked();
    void onAddButtonClicked();
    void onClearButtonClicked();
    void onAnchorChanged(const glm::vec2& pos);
    void onShapeAdded(const QString& label, const QString& type, const QColor& color, float thickness, const glm::vec2& anchor);
    void onShapesCleared();

private:
    QWidget* m_centralWidget;
    GLWidget* m_glWidget;

    QComboBox* m_shapeCombo;
    QPushButton* m_colorButton;
    QLineEdit* m_labelEdit;
    QDoubleSpinBox* m_thicknessSpin;
    QLineEdit* m_latEdit;
    QLineEdit* m_lonEdit;
    QPushButton* m_addButton;
    QPushButton* m_clearButton;
    QTableWidget* m_tableWidget;

    QColor m_currentColor;
};