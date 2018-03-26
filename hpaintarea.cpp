#include "hpaintarea.h"
#include <QDebug>
#include <QPaintEvent>
#include <QPainter>

HPaintArea::HPaintArea(QWidget* parent)
  : QWidget(parent)
{

  QFont newFont = font();
  newFont.setPixelSize(12);
  setFont(newFont);

  QFontMetrics fontMetrics(newFont);
  xBoundingRect = fontMetrics.boundingRect(tr("x"));
  yBoundingRect = fontMetrics.boundingRect(tr("y"));
  this->setCursor(Qt::IBeamCursor);
  this->grabKeyboard();
}
void
HPaintArea::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(Qt::black);
  QRect rect = QRect(0, 0, 10, 10);
  painter.fillRect(QRect(0, 0, 10, 10), QBrush(Qt::black));
  qDebug() << this->keyboardGrabber();
  qDebug() << this->mouseGrabber();
}
void
HPaintArea::inputMethodEvent(QInputMethodEvent* event)
{
  qDebug() << event;
}
void
HPaintArea::keyPressEvent(QKeyEvent* ev)
{
  qDebug() << ev;
}

void
HPaintArea::mousePressEvent(QMouseEvent* event)
{
  qDebug() << event;
}
