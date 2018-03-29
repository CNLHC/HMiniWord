#include "mainwindow.h"
#include "htextedit.h"
#include "ui_mainwindow.h"
#include <QComboBox>
#include <QDebug>
#include <QGridLayout>
#include <QScrollArea>
#include <QTextEdit>
MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
{
  this->resize(600, 600);
  auto htextedit = new HTextEdit(this);
  mHTextEdit = htextedit;
  setCentralWidget(mHTextEdit);
  setupMenuBar();
}

void
MainWindow::setupMenuBar()
{
  QMenu* menu = menuBar()->addMenu(tr("&File"));
  menu->addAction(tr("Save "), this, &MainWindow::save);
  menu->addAction(tr("Load ..."), this, &MainWindow::load);
  menu->addSeparator();
  menu->addAction(tr("&Quit"), this, &QWidget::close);
}
MainWindow::~MainWindow()
{
}
void
MainWindow::save()
{
}
void
MainWindow::load()
{
}
