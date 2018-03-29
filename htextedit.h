#ifndef HTEXTEDIT_H
#define HTEXTEDIT_H

#include "hpaintarea.h"
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
    if (ev->text().size() != 0) {
      this->mPaintArea->mController->LineUpdateAdd(0, 0, ev->text());
    }
  }
  void mouseMoveEvent(QMouseEvent* event)
  {
    qDebug() << mPaintArea->point2Coord((event->localPos()));
    qDebug() << event->localPos();
  }

  QSize sizeHint() { return QSize(600, 600); }
};

#endif // HTEXTEDIT_H
