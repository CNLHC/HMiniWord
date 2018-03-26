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
  explicit MainWindow(QWidget* parent = 0);

  ~MainWindow();

private:
  QMenu* mainWindowMenu;
  QMenu* dockWidgetMenu;
  void setupMenuBar();
  void save();
  void load();
};

#endif // MAINWINDOW_H
