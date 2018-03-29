#include "hpaintarea.h"
#include "hrendercontroller.h"
#include "htextcursor.h"
#include <QDebug>
#include <QPaintEvent>
#include <QPainter>

HPaintArea::HPaintArea(QWidget* parent, HTextCursor* cursor)
  : QFrame(parent)
  , mParent(parent)
{
  this->resize(parent->size());
  this->setCursor(Qt::IBeamCursor);
  this->mCursor = cursor;
  this->mController = new HRenderController(this);
}
HPaintArea::~HPaintArea()
{
  delete mController;
}

void
HPaintArea::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(Qt::black);
  auto font = this->mController->mFont;
  painter.setFont(font);
  for (auto i = this->mController->mScreenLine.begin();
       i != this->mController->mScreenLine.end(); i++) {
    painter.drawText((*i)->mRenderPos, (*i)->mString);
  }
}
void
HPaintArea::keyPressEvent(QKeyEvent* ev)
{
}
void
HPaintArea::mousePressEvent(QMouseEvent* event)
{
}

QPair<int, int>
HPaintArea::point2Coord(QPointF point)
{
  int lineInterval = this->mController->mLineinterval;
  int lineHeight = QFontMetrics(this->mController->mFont).height();
  int maxH = (lineInterval + lineHeight) * mController->mScreenLine.size();
  int row = 0, column = 0;

  if (point.y() > maxH) {
    row = mController->mScreenLine.size() - 1;
    row = row > 0 ? row : 0;
  } else if (static_cast<int>(point.y()) % (lineInterval + lineHeight) == 0)
    row = (point.y() - 1) / (lineInterval + lineHeight);
  else
    row = point.y() > 0 ? point.y() / (lineInterval + lineHeight) : 0;

  if (mController->mScreenLine.size() == 0) {
    column = 0;
  } else if (point.x() >= mController->mScreenLine[row]->mWidthList.back())
    column = mController->mScreenLine[row]->mWidthList.size() - 1;
  else {
    auto i = mController->mScreenLine[row]->mWidthList.begin();
    for (; i != mController->mScreenLine[row]->mWidthList.end(); i++)
      if (point.x() < (*i) + mController->xLeftOffset) {
        column = i - mController->mScreenLine[row]->mWidthList.begin();
        break;
      }
  }

  return qMakePair(row, column);
}
