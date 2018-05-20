#include "htextedit.h"
#include "hpaintarea.h"
#include "htextcursor.h"
#include <QScrollArea>
#include <QScrollBar>

HTextEdit::HTextEdit(QWidget* parent)
  : QWidget(parent)
  , mParent(parent)
{
  mScrollView = new QScrollArea(this);
  mPaintArea = new HPaintArea(mScrollView);
  mCursor = new HTextCursor(mScrollView, mPaintArea->getController());

  mScrollView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  mScrollView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  QScrollBar* m = mScrollView->verticalScrollBar();
  m->setStyleSheet("QScrollBar {width:15px;}");

  mScrollView->setWidget(mPaintArea);
  mCursor->stackUnder(mPaintArea);

  mScrollView->widget()->resize(parent->size());
  mScrollView->resize(parent->size());
  mCursor->resize(parent->size());
  resize(parent->size());

  QObject::connect(mPaintArea->mController, SIGNAL(lineExceed()), this,
                   SLOT(resizeTextArea()));
  QObject::connect(this->mCursor, SIGNAL(cursorOverflow()), this,
                   SLOT(autoScrollTextArea()));
  QObject::connect(mScrollView->verticalScrollBar(), SIGNAL(valueChanged(int)),
                   this->mCursor, SLOT(verScrollBarMove(int)));
  QObject::connect(mScrollView->verticalScrollBar(), SIGNAL(valueChanged(int)),
                   this->mPaintArea, SLOT(verScrollBarMove(int)));

  mPaintArea->mController->LineNew(0, "");
  mCursor->setPos(0, -1);
  mScrollView->verticalScrollBar()->setValue(
    1); //触发valueChanged信号，帮助子元件设置初值
  mScrollView->verticalScrollBar()->setValue(0);
  this->grabKeyboard();
}
void
HTextEdit::resizeTextArea()
{
  this->mPaintArea->resize(size().width(),
                           mPaintArea->size().height() + size().height() / 2);
  int value = this->mScrollView->verticalScrollBar()->value();
  this->mScrollView->verticalScrollBar()->setValue(value + size().height() / 2);
}
void
HTextEdit::autoScrollTextArea()
{
  int value = this->mScrollView->verticalScrollBar()->value();
  int curPaintHeight = this->mPaintArea->size().height();
  int curHeight = this->size().height();
  if (curPaintHeight - curHeight > curHeight / 2)
    this->mScrollView->verticalScrollBar()->setValue(value +
                                                     size().height() / 2);
  else
    this->mScrollView->verticalScrollBar()->setValue(
      value + (curPaintHeight - curHeight));
}
