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
  QObject::connect(this->findButton, SIGNAL(clicked(bool)), this,
                   SLOT(findSub()));
  isFindProcessing = false;
}

void
HTextEditFindView::findSub()
{
  qDebug() << "find";
  qDebug() << mCurResult;
  if (!isFindProcessing) {
    CurResultIndex = 0;
    isFindProcessing = true;
    mFindSession->initNewSession(findInput->text());
    findButton->setText("wait...");
    findButton->setEnabled(false);
    findInput->setEnabled(false);
    mProgressBar->setVisible(true);
    mFindSession->run();
  } else {
    if (CurResultIndex > mCurResult.length() - 1) {
      qDebug() << "find over";
      isFindProcessing = false;
      findButton->setText("查找");
      mCurResult.clear();
      mProgressBar->setVisible(false);
      mProgressBar->setFormat("%p");
      mProgressBar->setValue(0);
      return;
    }
    auto tCursor = mHTextEdit->mCursor;
    int tRow = mCurResult[CurResultIndex].first;
    int tCol = mCurResult[CurResultIndex].second;
    tCursor->setPos(tRow, tCol, tRow, tCol + findInput->text().length() - 1);
    mProgressBar->setFormat(QVariant(CurResultIndex + 1).toString() +
                            QString("/") +
                            QVariant(mCurResult.length()).toString());
    emit findProcess(CurResultIndex * 100 / mCurResult.length());
    emit findResultCursorChange();
    CurResultIndex++;
  }
}
void
HTextEditFindView::findOverCallback()
{
  findButton->setText("next");
  findButton->setEnabled(true);
  findInput->setEnabled(true);
  mProgressBar->setValue(0);
  mCurResult = mFindSession->getResult();
  findSub();
}

void
HTextEditFindView::findInputChanged(QString fstr)
{
  isFindProcessing = false;
  findButton->setText("查找");
  if (fstr.length() > 0)
    findButton->setEnabled(true);
  else
    findButton->setEnabled(false);
}
