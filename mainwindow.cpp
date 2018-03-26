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
  setWindowTitle("MiniWord");
  paintScrollArea->setWidget(paintArea);
  paintScrollArea->setWidgetResizable(true);
  paintScrollArea->widget()->resize(560, 560);
  paintScrollArea->resize(560, 560);
  setCentralWidget(paintScrollArea);
  this->resize(560, 560);
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
void
MainWindow::resizeEvent(QResizeEvent* event)
{
  qDebug() << event;
}
