#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "hdocumentfind.h"
#include "htextedit.h"
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
  void findSub();
  bool save();
  void NewFile();

private:
  Ui::MainWindow* ui;
  QVBoxLayout* mMainLayout;
  QVBoxLayout* FindSubLayout;
  QMenu* mainWindowMenu;
  QMenu* dockWidgetMenu;
  QLineEdit* findInput;
  QLineEdit* subInput;
  QPushButton* findButton;
  QPushButton* subButton;
  QProgressBar* mProgressBar;

  bool isFindActivated;
  void SetFindDialogVisible(bool bisiblity);
  void setupMenuBar();
  bool saveAs();
  void load();
  bool changedWarning();

  hdocumentfind* curFindSession;
  HTextEdit* mHTextEdit;
};

#endif // MAINWINDOW_H
