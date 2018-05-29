#include "htexteditfindview.h"

HTextEditFindView::HTextEditFindView(QWidget* parent)
  : QWidget(parent)
{
  findInput = new QLineEdit;
  subInput = new QLineEdit;
  findButton = new QPushButton;
  subButton = new QPushButton;
  mProgressBar = new QProgressBar(this);

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

  //  QObject::connect(mFindSession, SIGNAL(searchProgress(int)),
  //                   this->mProgressBar, SLOT(setValue(int)));
  //  QObject::connect(this->findInput, SIGNAL(textChanged(QString)), this,
  //                   SLOT(findInputChanged(QString)));
  //  QObject::connect(this->findButton, SIGNAL(clicked(bool)), this,
  //                   SLOT(findSub()));
}
