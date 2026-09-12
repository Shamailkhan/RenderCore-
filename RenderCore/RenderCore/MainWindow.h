#pragma once

#include <QMainWindow>

class GLCanvas;
class DrawingControlPanel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    GLCanvas* m_canvas = nullptr;
    DrawingControlPanel* m_panel = nullptr;
};
