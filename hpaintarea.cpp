#include "hpaintarea.h"
#include "hrendercontroller.h"
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
  mCursor = new HTextCursor(parent);

  QFontMetrics fontMetrics(newFont);
  xBoundingRect = fontMetrics.boundingRect(tr("x"));
  yBoundingRect = fontMetrics.boundingRect(tr("y"));
  this->setCursor(Qt::IBeamCursor);
  this->grabKeyboard();

  this->mController = new HRenderController(this);
}
HPaintArea::~HPaintArea()
{
  delete mCursor;
  delete mController;
}

void
HPaintArea::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(Qt::black);
  auto font = QFont("Arial", 30);
  painter.setFont(font);
  painter.drawText(30, 30, a);
}
void
HPaintArea::inputMethodEvent(QInputMethodEvent* event)
{
  qDebug() << event;
}
void
HPaintArea::keyPressEvent(QKeyEvent* ev)
{
  qDebug() << "Mocking To Model:" << ev->text();
  // Mocking , update slot should be connect to the model change siganl
  a.append(ev->text());
  this->update();
  auto font = QFont("Arial", 30);
  QFontMetrics fm(font);
  mCursor->setPos(30 + fm.width(a), 10);
  this->mController->LineNew(0, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                                "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                                "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                                "aaaaaaaaaaaaaa");
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
