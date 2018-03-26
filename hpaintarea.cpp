#include "hpaintarea.h"
#include "htextcursor.h"
#include <QDebug>
#include <QPaintEvent>
#include <QPainter>

HPaintArea::HPaintArea(QWidget* parent)
  : QFrame(parent)
{

  QFont newFont = font();
  newFont.setPixelSize(12);
  setFont(newFont);

  mCursor = new HTextCursor(this);
  mCursor->setVisible(true);
  mCursor->update();
  QFontMetrics fontMetrics(newFont);
  xBoundingRect = fontMetrics.boundingRect(tr("x"));
  yBoundingRect = fontMetrics.boundingRect(tr("y"));
  this->setCursor(Qt::IBeamCursor);
  this->grabKeyboard();
}
HPaintArea::~HPaintArea()
{
  delete mCursor;
}
void
HPaintArea::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);
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

void
HPaintArea::resizeEvent(QResizeEvent* event)
{
  qDebug() << event << "(Text Area)";
}
