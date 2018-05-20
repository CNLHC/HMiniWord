#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "htextedit.h"
#include <QMainWindow>
#include <QResizeEvent>

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
  bool save();
  bool saveAs();
  void load();
  void NewFile();
  bool changedWarning();
  void findSub();
  HTextEdit* mHTextEdit;
};

#endif // MAINWINDOW_H
