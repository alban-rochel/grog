#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "grog.h"
#include "Engine.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void generateTrigo();

  protected slots:
    /*void animate();*/
    void debug();

    void eyeXChanged(double);
    void eyeYChanged(double);
    void eyeZChanged(double);

    void targetXChanged(double);
    void targetYChanged(double);
    void targetZChanged(double);

    void upXChanged(double);
    void upYChanged(double);
    void upZChanged(double);

    void fovChanged(double);
    void nearChanged(double);
    void farChanged(double);

    void defaultScene();
    void convertObj();

  private:
    Ui::MainWindow *ui;

    double eyeX {/*-2.*/2.60}, eyeY {1.}, eyeZ {-0.4};
    double targetX {0.}, targetY {0.}, targetZ {0.};
    double upX {0.}, upY {-1.}, upZ {0.};
    double fov {1.}, near {.001}, far {10.};

    grog::SceneNode scene {};
    grog::Engine engine {};
    QPixmap pix {80, 64};

    void draw();

};

#endif // MAINWINDOW_H
