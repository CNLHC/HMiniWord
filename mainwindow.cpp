#include "mainwindow.h"
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
#include <QScrollArea>
#include <QShortcut>
#include <QTextEdit>
#include <QThread>
#include <QVBoxLayout>
namespace UI {
}
MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
{

  this->resize(700, 600);
  this->setMinimumWidth(600);
  this->setMaximumWidth(600);

  mMainLayout = new QVBoxLayout;
  mHTextEdit = new HTextEdit(this);
  mHTextFind = new HTextEditFindView(mHTextEdit, this);
  mHTextFind->setVisible(false);

  QWidget* window = new QWidget();

  mMainLayout->addWidget(mHTextEdit);
  mMainLayout->addWidget(mHTextFind);

  window->setLayout(mMainLayout);

  setTabOrder(mHTextEdit, mHTextFind);
  setTabOrder(mHTextFind, mHTextEdit);
  new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_F), this,
                SLOT(ToggleFindDialog()));
  new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this, SLOT(save()));
  new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_N), this, SLOT(NewFile()));

  QObject::connect(mHTextFind, SIGNAL(findResultCursorChange()), mHTextEdit,
                   SLOT(ScrollTextAreaByCursor()));
  setCentralWidget(window);
  setupMenuBar();
}

void
MainWindow::setupMenuBar()
{
  QMenu* fileMenu = menuBar()->addMenu(tr("&文件"));
  fileMenu->addAction(tr("新建..."), this, &MainWindow::NewFile);
  fileMenu->addAction(tr("打开..."), this, &MainWindow::load);
  fileMenu->addAction(tr("保存..."), this, &MainWindow::save);
  fileMenu->addAction(tr("另存为..."), this, &MainWindow::save);
  fileMenu->addSeparator();
  fileMenu->addAction(tr("&退出"), this, &QWidget::close);
  QMenu* toolMenu = menuBar()->addMenu(tr("工具"));
  toolMenu->addAction(tr("查找与替换"), this, &MainWindow::ToggleFindDialog);
}
MainWindow::~MainWindow()
{
}
bool
MainWindow::save()
{
  if (this->mHTextEdit->filePath.size() == 0)
    mHTextEdit->filePath =
      QFileDialog::getSaveFileName(this, tr("保存工作区"), "", tr("*"));
  if (this->mHTextEdit->filePath.size() == 0)
    return false;
  return mHTextEdit->dumpToFile();
}
bool
MainWindow::saveAs()
{
  QString fp =
    QFileDialog::getSaveFileName(this, tr("工作区另存为"), "", tr("*"));
  if (fp.size() == 0)
    return false;
  mHTextEdit->filePath = fp;
  return mHTextEdit->dumpToFile();
}
void
MainWindow::load()
{
  if (!changedWarning())
    return;
  QString fpath =
    QFileDialog::getOpenFileName(this, tr("打开文件"), "", tr("*"));
  if (fpath.size() == 0)
    return;
  mHTextEdit->loadFromFile(fpath);
}
bool
MainWindow::changedWarning()
{
  if (mHTextEdit->isChanged()) {
    QMessageBox msgBox;
    msgBox.setText(tr("文档已经改变"));
    msgBox.setInformativeText(tr("你想要保存更改吗？"));
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard |
                              QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();
    switch (ret) {
      case QMessageBox::Save:
        if (save())
          return true;
        else
          return false;
        break;
      case QMessageBox::Discard:
        return true;
        break;
      case QMessageBox::Cancel:
        return false;
        break;
      default:
        break;
    }
  }
  return true;
}
void
MainWindow::NewFile()
{
  if (!changedWarning())
    return;
  mHTextEdit->filePath =
    QFileDialog::getSaveFileName(this, tr("选择文件名"), "", tr("*"));

  mHTextEdit->InitialBuf();
}

void
MainWindow::ToggleFindDialog()
{
  mHTextFind->setVisible(!mHTextFind->isVisible());
}
