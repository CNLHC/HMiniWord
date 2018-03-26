#include "htextcursor.h"

#include <QDebug>
#include <QObject>
#include <QTimer>
void
HTextCursor::blink()
{
  QTimer* timer = new QTimer(0);
  QObject::connect(timer, SIGNAL(timeout()), this, SLOT(toggleBlink()));
  timer->start(1000);
  qDebug() << "reg change state";
}
void
HTextCursor::toggleBlink()
{
  qDebug() << "Toggle Blink";
  blink();
}
