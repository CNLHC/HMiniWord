#ifndef HTEXTEDIT_H
#define HTEXTEDIT_H

#include "hpaintarea.h"
#include "hrendercontroller.h"
#include "htextcursor.h"
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QScrollArea>
#include <QScrollBar>
#include <QWidget>
#include <QtGlobal>
#include <cctype>

class HTextEdit : public QWidget
{
  Q_OBJECT
public:
  explicit HTextEdit(QWidget* parent = nullptr);
  QWidget* mParent;
  QString filePath;
  /*!
   * \brief 返回当前文档在上次修改后是否变更过
   */
  bool isChanged() { return mChangedFlag; }
  /*!
   * \brief 标记当前文档已经被保存过
   */
  void setSaveFlag() { mChangedFlag = false; }
  bool dumpToFile()
  {
    QFile file(this->filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
      QMessageBox msgBox(this);
      msgBox.setText(tr("文件保存失败:无法打开文件"));
      msgBox.exec();
      return false;
    }
    QTextStream out(&file);
    auto model = mPaintArea->mController->getModel();
    for (auto i = 0; i < model->getLogicLineSize(); i++)
      out << (model->composeLogicLine(i)->toUtf8()) << endl;
    setSaveFlag();
    file.close();
    return true;
  }
  bool loadFromFile(QString path)
  {
    clearCurBuffer();
    auto ctr = this->mPaintArea->mController;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      QMessageBox msgBox(this);
      msgBox.setText(tr("文件打开失败:无法打开文件"));
      msgBox.exec();
      InitialBuf();
      return false;
    }
    filePath = path;
    QTextStream in(&file);
    while (!in.atEnd()) {
      QString line = in.readLine();
      ctr->LineNew(line);
    }
    setSaveFlag();
    return true;
  }
  void InitialBuf()
  {
    clearCurBuffer();
    mPaintArea->mController->LineNew(0, "");
    mCursor->setPos(0, -1);
  }

signals:
  void printableKeyPress();

public slots:
  void resizeTextArea();
  void autoScrollTextArea();
  void setChangedStatus() { mChangedFlag = true; }

private:
  HTextCursor* mCursor;
  HPaintArea* mPaintArea;
  bool mChangedFlag;
  QScrollArea* mScrollView;
  QPair<int, int> tempPoint;
  void clearCurBuffer()
  {
    auto ctr = this->mPaintArea->mController;
    while (ctr->mLogicLine.size() > 0)
      ctr->LineDelete(0);
  }
  void resizeEvent(QResizeEvent* event)
  {
    mCursor->resize(event->size());
    mScrollView->resize(event->size());
    int oH = mPaintArea->size().height();
    mPaintArea->resize(event->size().width(), oH);
  }
  void wheelEvent(QWheelEvent* ev)
  {
    auto currentValue = mScrollView->verticalScrollBar()->value();
    auto MaxValue = mScrollView->verticalScrollBar()->maximum();
    auto MinValue = mScrollView->verticalScrollBar()->minimum();
    if (ev->angleDelta().y() < 0)
      mScrollView->verticalScrollBar()->setValue(
        qMin(currentValue + 50, MaxValue));
    else
      mScrollView->verticalScrollBar()->setValue(
        qMax(currentValue - 50, MinValue));
  }
  void keyPressEvent(QKeyEvent* ev)
  {
    mCursor->Closeblink();
    auto cor = mCursor->getPriCursor();
    auto controller = mPaintArea->mController;
    if (ev->key() == Qt::Key_Backspace) {
      int contextLine = controller->mScreenLine.size();
      int pos = controller->SL2LL(cor.first);
      int column = controller->SLC2LLC(cor.first, cor.second);
      qDebug() << "del line from Controller" << pos;
      if (column == -1) {
        controller->LineDelete(pos);

        if (controller->mLogicLine.size() ==
            0) { //如果删除完后无其他行，那么新建一个空行。
          mPaintArea->mController->LineNew(0, "");
          mCursor->setPos(0, -1);
        } else if (pos ==
                   0) { //如果删除的是第一行，那么将光标置于新的第一行的行头
          mCursor->setPos(0, -1);
        } else { //其余情况将光标置于上一行的行尾
          int SLindex =
            controller->LL2LastSL(controller->mLogicLine.begin() + pos - 1);
          mCursor->setPos(SLindex,
                          controller->mScreenLine[SLindex]->mString.size() - 1);
        }

        return;
      }
      controller->LineUpdateDelete(pos, column - 1, column - 1);
      if (controller->mScreenLine.size() - contextLine < 0)
        mCursor->setPos(cor.first - 1,
                        controller->mScreenLine[cor.first - 1]->mString.size() -
                          1);
      else
        mCursor->setPos(cor.first, cor.second - 1);
    } else if (ev->key() == Qt::Key_Return) {
      int pos = controller->SL2LL(cor.first);
      controller->LineNew(pos + 1, QString(""));
      mCursor->setPos(
        controller->LL2LastSL(controller->mLogicLine.begin() + pos + 1), -1);

    } else if (ev->key() == Qt::Key_Delete) {
      int contextLine = controller->mScreenLine.size();
      int pos = controller->SL2LL(cor.first);
      int column = controller->SLC2LLC(cor.first, cor.second);
      controller->LineUpdateDelete(pos, column, column);
      if (controller->mScreenLine.size() - contextLine < 0)
        mCursor->setPos(cor.first - 1,
                        controller->mScreenLine[cor.first - 1]->mString.size() -
                          1);
      else
        mCursor->setPos(cor.first, cor.second - 1);
    } else if (ev->key() == Qt::Key_Up) {
      mCursor->move(HTextCursor::cursorUp);
    } else if (ev->key() == Qt::Key_Down) {
      mCursor->move(HTextCursor::cursorDown);
    } else if (ev->key() == Qt::Key_Left) {
      mCursor->move(HTextCursor::cursorLeft);
    } else if (ev->key() == Qt::Key_Right) {
      mCursor->move(HTextCursor::cursorRight);
    } else if (ev->text().size() != 0) {
      int contextLine = controller->mScreenLine.size();
      int pos = controller->SL2LL(cor.first); //注意！插入操作是针对逻辑行的
      int column = controller->SLC2LLC(cor.first, cor.second);
      controller->LineUpdateAdd(pos, column + 1, ev->text());
      if (controller->mScreenLine.size() - contextLine > 0)
        mCursor->setPos(cor.first + 1, 0);
      else
        mCursor->setPos(cor.first, cor.second + 1);
    }
    mCursor->blink();
  }
  void mousePressEvent(QMouseEvent* event)
  {
    qDebug() << "cursor size" << mCursor->size();
    qDebug() << "paintarea size" << mPaintArea->size();
    qDebug() << "scroll size" << mScrollView->size();
    qDebug() << "Mouse Pos" << event;

    auto p = mPaintArea->point2Coord(event->localPos());
    tempPoint = p;
    qDebug() << "row" << p;
    this->mCursor->setPos(p.first, p.second, p.first, p.second);
  }
  void mouseMoveEvent(QMouseEvent* event)
  {
    auto p = mPaintArea->point2Coord(event->localPos());
    this->mCursor->setPos(tempPoint.first, tempPoint.second, p.first, p.second);
  }
  void mouseReleaseEvent(QMouseEvent* event) {}
  QSize sizeHint() { return QSize(600, 600); }
};

#endif // HTEXTEDIT_H
