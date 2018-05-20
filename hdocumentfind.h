#ifndef HDOCUMENTFIND_H
#define HDOCUMENTFIND_H

#include "hrendercontroller.h"
#include <QDebug>
#include <QList>
#include <QObject>
#include <QPair>
#include <QThread>
#include <QWidget>
#include <QtGlobal>

class hdocumentfind : public QThread
{
  Q_OBJECT
private:
  QString fstr;
  HRenderController* mController;
  QList<QPair<int, int>> result;

public:
  hdocumentfind(QString findStr, HRenderController* controller)
    : fstr(findStr)
    , mController(controller)
  {
  }
  void run() override
  {
    result.clear();
    int ScreenLineSize = mController->mScreenLine.size();
    for (int i = 0; i < ScreenLineSize; i++) {
      QString& tStr = mController->mScreenLine[i]->mString;
      int j = 0;
      while ((j = tStr.indexOf(fstr, j)) != -1)
        result.append(qMakePair(i, j++));
      int progress = i * 100 / ScreenLineSize;
      emit searchProgress(progress);
    }
    emit searchProgress(100);
    emit searchOver();
  }
signals:
  void searchProgress(int);
  void searchOver();
};

#endif // HDOCUMENTFIND_H
