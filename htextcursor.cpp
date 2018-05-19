#include "htextcursor.h"

#include <QDebug>
#include <QObject>
#include <QPainter>
#include <QTimer>
#include <QtGlobal>
HTextCursor::HTextCursor(QWidget* parent, HRenderController* controller)
  : QWidget(parent)
  , mParent(parent)
  , mController(controller)
{
  BlinkTimer = new QTimer(0);
  QObject::connect(BlinkTimer, SIGNAL(timeout()), this, SLOT(toggleBlink()));
  this->resize(parent->size());
}
void
HTextCursor::blink()
{
  BlinkTimer->start(500);
}
void
HTextCursor::Closeblink()
{
  BlinkTimer->stop();
  mCurColor = mColor1;
}
void
HTextCursor::setCursor(int row1, int column1, int row2, int column2)
{
  int marow, macolumn, mirow, micolumn;
  if (row1 * 10 + column1 >= row2 * 10 + column2) {
    marow = row1;
    macolumn = column1;
    mirow = row2;
    micolumn = column2;
  } else if (row1 * 10 + column1 < row2 * 10 + column2) {
    marow = row2;
    macolumn = column2;
    mirow = row1;
    micolumn = column1;
  }
  priCursor = qMakePair(marow, macolumn);
  altCursor = qMakePair(mirow, micolumn);
}
void
HTextCursor::setPos(int row1, int column1)
{
  setPos(row1, column1, row1, column1);
}
void
HTextCursor::setPos(int row1, int column1, int row2, int column2)
{
  if (column1 < -1)
    column1 = -1;
  if (column2 < -1)
    column2 = -1;
  this->renderAera.clear();
  this->setCursor(row1, column1, row2, column2);

  const int offset = mController->xLeftOffset;
  const int height = (mController->mLineinterval + mController->mLineHeight);
  int beginx, lastx;
  //切换闪烁状态:在多选时不闪烁游标
  if (row1 == row2 && column1 == column2)
    this->blink();
  else
    this->Closeblink();

  //通过row和column获取宽度
  auto col2width = [this, offset](int row, int col) {
    Q_ASSERT(row < mController->mScreenLine.size());
    if (col < 0)
      return 0;
    else
      return mController->mScreenLine[row]->mWidthList[col];
  };

  for (int i = altCursor.first; i <= priCursor.first; i++) {
    if (!mController->isBlankLine(i)) {
      beginx =
        (i == altCursor.first) ? col2width(i, altCursor.second - 1) : offset;
      lastx = (i == priCursor.first)
                ? col2width(i, priCursor.second)
                : col2width(i, mController->mScreenLine[i]->mString.size() - 1);
    } else {
      beginx = lastx = 0;
    }

    if (beginx == lastx)
      lastx += 2;
    renderAera.append(QRect(QPoint(beginx + offset + 1, (i + 0.2) * height),
                            QPoint(lastx + offset, (i + 1) * height)));
  }
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
  Q_UNUSED(event);
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(Qt::black);
  for (int i = 0; i < renderAera.size(); i++)
    painter.fillRect(renderAera[i], QBrush(this->mCurColor));
}
