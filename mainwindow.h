#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_pushButton_clicked();

private:

  QString libconfigToJson(QString libconfigStr);

  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H