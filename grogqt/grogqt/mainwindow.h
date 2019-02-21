#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "grog.h"

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
    void animate();
    void fastInteger() noexcept;

  private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
