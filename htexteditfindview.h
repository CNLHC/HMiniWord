#ifndef HTEXTEDITFINDVIEW_H
#define HTEXTEDITFINDVIEW_H
#include "hdocumentfind.h"
#include "htextedit.h"
#include <QLineEdit>
#include <QMessageBox>
#include <QProgressBar>
#include <QPushButton>
#include <QScrollArea>
#include <QShortcut>
#include <QTextEdit>
#include <QThread>
#include <QVBoxLayout>
#include <QWidget>

class HTextEditFindView : public QWidget
{
  Q_OBJECT
public:
  explicit HTextEditFindView(QWidget* parent = nullptr);

private:
  HTextEdit* mHTextEdit;
  QLineEdit* findInput;
  QLineEdit* subInput;
  QPushButton* findButton;
  QPushButton* subButton;
  QProgressBar* mProgressBar;
  hdocumentfind* mFindSession;
  QVBoxLayout* FindSubLayout;

  bool isFindUIActivated;
  bool isFindProcessing;

signals:

private slots:
  //  void findInputChanged(QString fstr);
  //  void findSub();
};

#endif // HTEXTEDITFINDVIEW_H
