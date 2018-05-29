#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "htextedit.h"
#include "htexteditfindview.h"
#include "ui_mainwindow.h"
#include <QComboBox>
#include <QDebug>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QProgressBar>
#include <QPushButton>
#include <QScrollArea>
#include <QShortcut>
#include <QTextEdit>
#include <QThread>
#include <QVBoxLayout>

namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = 0);

  ~MainWindow();

private slots:
  void ToggleFindDialog();
  bool save();
  void NewFile();

private:
  Ui::MainWindow* ui;
  HTextEdit* mHTextEdit;
  HTextEditFindView* mHTextFind;
  QVBoxLayout* mMainLayout;
  QMenu* mainWindowMenu;
  QMenu* dockWidgetMenu;
  void setupMenuBar();
  bool saveAs();
  void load();
  bool changedWarning();
  void resizeEvent(QResizeEvent* ev) { qDebug() << ev; }
};

#endif // MAINWINDOW_H
