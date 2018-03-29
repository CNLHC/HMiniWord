#include "htextcursor.h"

#include <QDebug>
#include <QObject>
#include <QPainter>
#include <QTimer>
HTextCursor::HTextCursor(QWidget* parent)
  : mParent(parent)
  , QWidget(parent)
{
  blink();
  this->resize(parent->size());
}
void
HTextCursor::blink()
{
  QTimer* timer = new QTimer(0);
  QObject::connect(timer, SIGNAL(timeout()), this, SLOT(toggleBlink()));
  timer->start(500);
}
void
HTextCursor::setPos(int x, int y)
{
  mx = x;
  my = y;
  this->update();
}

void
HTextCursor::toggleBlink()
{
  if (mCurColor == mColor1)
    mCurColor = mColor2;
  else
    mCurColor = mColor1;
  this->update();
}
void
HTextCursor::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(Qt::black);
  QRect rect = QRect(mx, my, 10, 20);
  painter.fillRect(rect, QBrush(this->mCurColor));
}
