#ifndef HTEXTEDIT_H
#define HTEXTEDIT_H

#include "hpaintarea.h"
#include "hrendercontroller.h"
#include "htextcursor.h"
#include <QScrollArea>
#include <QWidget>
#include <cctype>

class HTextEdit : public QWidget
{
  Q_OBJECT
public:
  explicit HTextEdit(QWidget* parent = nullptr);

  QWidget* mParent;
signals:
  void printableKeyPress();

public slots:
  void resizeTextArea();

private:
  HTextCursor* mCursor;
  HPaintArea* mPaintArea;
  QScrollArea* mScrollView;
  QPair<int, int> tempPoint;
  void resizeEvent(QResizeEvent* event)
  {
    qDebug() << "htext edit" << event;
    mCursor->resize(event->size());
    mPaintArea->resize(event->size());
    mParent->resize(event->size());
    mScrollView->resize(event->size());
  }
  void keyPressEvent(QKeyEvent* ev)
  {
    qDebug() << ev;
    auto cor = mCursor->getPriCursor();
    auto controller = mPaintArea->mController;
    if (ev->key() == Qt::Key_Backspace) {
      qDebug() << ev->count();
    } else if (ev->key() == Qt::Key_Return) {
      controller->LineNew(cor.first + 1, QString(""));
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
  }
  void mousePressEvent(QMouseEvent* event)
  {
    qDebug() << mPaintArea->point2Coord((event->localPos()));
    auto p = mPaintArea->point2Coord((event->localPos()));
    tempPoint = p;
    this->mCursor->setPos(p.first, p.second, p.first, p.second);
  }
  void mouseMoveEvent(QMouseEvent* event)
  {
    qDebug() << mPaintArea->point2Coord((event->localPos()));
    auto p = mPaintArea->point2Coord((event->localPos()));
    this->mCursor->setPos(tempPoint.first, tempPoint.second, p.first, p.second);
  }
  void mouseReleaseEvent(QMouseEvent* event)
  {
    qDebug() << mPaintArea->point2Coord((event->localPos()));
    qDebug() << event->localPos();
  }

  QSize sizeHint() { return QSize(600, 600); }
};

#endif // HTEXTEDIT_H
