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
}
void
HTextCursor::blink()
{
  QTimer* timer = new QTimer(0);
  QObject::connect(timer, SIGNAL(timeout()), this, SLOT(toggleBlink()));
  timer->start(500);
}
void
HTextCursor::toggleBlink()
{
  this->setVisible(!this->isVisible());
}
void
HTextCursor::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(Qt::black);
  QRect rect = QRect(0, 0, 10, 10);
  painter.fillRect(QRect(0, 0, 10, 10), QBrush(Qt::black));
}
