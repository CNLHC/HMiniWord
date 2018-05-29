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
    isFindProcessing = true;
    mFindSession->initNewSession(findInput->text());
    findButton->setText("wait...");
    findButton->setEnabled(false);
    findInput->setEnabled(false);
    mProgressBar->setVisible(true);
    mFindSession->run();
  }
}
void
HTextEditFindView::findOverCallback()
{
  findButton->setText("next");
  findButton->setEnabled(true);
  findInput->setEnabled(true);
  mProgressBar->setValue(0);
  mProgressBar->setFormat("find results");
  mCurResult = mFindSession->getResult();
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
