#include "mainwindow.h"
#include "hpaintarea.h"
#include "ui_mainwindow.h"
#include <QGridLayout>

#include <QComboBox>
#include <QDebug>
#include <QScrollArea>
#include <QTextEdit>
MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
{
  auto paintArea = new HPaintArea;
  auto paintScrollArea = new QScrollArea;
  setWindowTitle("Qt Main Window Example");

  paintScrollArea->setWidget(paintArea);
  paintScrollArea->widget()->resize(560, 560);
  setCentralWidget(paintScrollArea);
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
