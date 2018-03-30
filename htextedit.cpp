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
  QObject::connect(mPaintArea->mController, SIGNAL(lineExceed()), this,
                   SLOT(resizeTextArea()));

  mScrollView->widget()->resize(parent->size());
  mScrollView->resize(parent->size());
  mCursor->resize(parent->size());
  resize(parent->size());

  mPaintArea->mController->LineNew(0, "");

  this->grabKeyboard();
}
void
HTextEdit::resizeTextArea()
{
  this->mPaintArea->resize(size().width(),
                           this->mPaintArea->mController->maxHeight);
}
