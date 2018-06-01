#include "htexteditfindview.h"

HTextEditFindView::HTextEditFindView(HTextEdit* HTE, QWidget* parent)
  : QWidget(parent)
  , mHTextEdit(HTE)
{
  findInput = new QLineEdit;
  subInput = new QLineEdit;
  findButton = new QPushButton;
  subButton = new QPushButton;
  mProgressBar = new QProgressBar(this);
  mProgressBar->setVisible(false);
  mFindSession = new hdocumentfind(mHTextEdit->mPaintArea->mController);

  FindSubLayout = new QVBoxLayout;
  QHBoxLayout* findLayout = new QHBoxLayout;
  findLayout->addWidget(findInput);
  findLayout->addWidget(findButton);
  findButton->setText("查找");
  findButton->setEnabled(false);
  QHBoxLayout* subLayout = new QHBoxLayout;
  subLayout->addWidget(subInput);
  subLayout->addWidget(subButton);
  subButton->setText("替换");
  subButton->setEnabled(false);
  subButton->setVisible(false);
  subInput->setVisible(false);

  FindSubLayout->addWidget(mProgressBar);
  FindSubLayout->addLayout(findLayout);
  FindSubLayout->addLayout(subLayout);
  this->setLayout(FindSubLayout);

  QObject::connect(mFindSession, SIGNAL(searchProgress(int)),
                   this->mProgressBar, SLOT(setValue(int)));

  QObject::connect(this, SIGNAL(findProcess(int)), this->mProgressBar,
                   SLOT(setValue(int)));

  QObject::connect(mFindSession, SIGNAL(searchOver()), this,
                   SLOT(findOverCallback()));

  QObject::connect(this->findInput, SIGNAL(textChanged(QString)), this,
                   SLOT(findInputChanged(QString)));

  QObject::connect(this->subInput, SIGNAL(textChanged(QString)), this,
                   SLOT(subInputChanged(QString)));

  QObject::connect(this->findButton, SIGNAL(clicked(bool)), this,
                   SLOT(findSub()));

  QObject::connect(this->subButton, SIGNAL(clicked(bool)), this, SLOT(sub()));
  isFindProcessing = false;
}

void
HTextEditFindView::findSub()
{
  if (!isFindProcessing) { // New Session
    CurResultIndex = -1; // Start with -1, When the find down, findNext will be
                         // invoked and the initial value of this actually is 0.
    isFindProcessing = true;
    mFindSession->initNewSession(findInput->text());
    findButton->setText("wait...");
    findButton->setEnabled(false);
    findInput->setEnabled(false);
    mProgressBar->setVisible(true);
    mFindSession->run();
  } else
    findNext();
}
void
HTextEditFindView::findNext()
{
  CurResultIndex++;
  if (CurResultIndex > mCurResult.length() - 1) {
    isFindProcessing = false;
    findButton->setText("查找");
    mCurResult.clear();
    mProgressBar->setVisible(false);
    mProgressBar->setFormat("%p%");
    mProgressBar->setValue(0);
    subButton->setVisible(false);
    subInput->setVisible(false);
    return;
  }
  auto tCursor = mHTextEdit->mCursor;
  int tRow = mCurResult[CurResultIndex].first;
  int tCol = mCurResult[CurResultIndex].second;
  tCursor->setPos(tRow, tCol, tRow, tCol + findInput->text().length() - 1);

  mProgressBar->setFormat(QVariant(CurResultIndex + 1).toString() +
                          QString("/") +
                          QVariant(mCurResult.length()).toString());

  emit findProcess((1 + CurResultIndex) * 100 / mCurResult.length());
  emit findResultCursorChange();
}
void
HTextEditFindView::findOverCallback()
{
  findButton->setText("next");
  findButton->setEnabled(true);
  findInput->setEnabled(true);
  mProgressBar->setValue(0);
  mCurResult = mFindSession->getResult();
  if (mCurResult.length() > 0) {
    subButton->setVisible(true);
    subInput->setVisible(true);
  }
  findSub();
}

void
HTextEditFindView::findInputChanged(QString fstr)
{
  isFindProcessing = false;
  subButton->setVisible(false);
  subInput->setVisible(false);
  findButton->setText("查找");
  if (fstr.length() > 0)
    findButton->setEnabled(true);
  else
    findButton->setEnabled(false);
}

void
HTextEditFindView::subInputChanged(QString fstr)
{
  subButton->setText("替换");
  if (fstr.length() > 0)
    subButton->setEnabled(true);
  else
    subButton->setEnabled(false);
}

void
HTextEditFindView::sub()
{
  QString sstr = subInput->text();
  if (CurResultIndex > mCurResult.length())
    return;
  auto tCursor = mHTextEdit->mCursor;
  auto ctr = mHTextEdit->mPaintArea->getController();
  int tRow = mCurResult[CurResultIndex].first;
  int tCol = mCurResult[CurResultIndex].second;
  int tColLL = ctr->SLC2LLC(tRow, tCol);
  int tRowLL = ctr->SL2LL(tRow);
  ctr->LineUpdateDelete(tRowLL, tColLL,
                        tColLL + findInput->text().length() - 1);
  ctr->LineUpdateAdd(tRowLL, tColLL, sstr);
  tCursor->setPos(tRow, tCol, tRow, tCol + sstr.length() - 1);
  findNext();
}
