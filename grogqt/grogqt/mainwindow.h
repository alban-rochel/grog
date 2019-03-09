#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "grog.h"
#include "Engine.h"
#include "Colors.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QPixmap drawImage(grog::bufferType* buffer) noexcept;

  protected slots:
    /*void animate();
    void debug();*/

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

  private:
    Ui::MainWindow *ui;

    double eyeX {1.}, eyeY {1.}, eyeZ {1.};
    double targetX {0.}, targetY {0.}, targetZ {0.};
    double upX {0.}, upY {1.}, upZ {0.};
    double fov {2.}, near {1.}, far {10.};

    grog::SceneNode scene {};
    grog::Engine engine {};
    QPixmap pix {80, 64};

    void draw();

};

#endif // MAINWINDOW_H
