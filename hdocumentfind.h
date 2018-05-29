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
  bool isSessionReady;

public:
  hdocumentfind(HRenderController* controller)
    : mController(controller)
  {
    isSessionReady = false;
  }
  void initNewSession(QString fstr)
  {
    Q_ASSERT(fstr.length() > 0 && "The search string must't an empty string");
    if (this->isRunning()) {
      this->terminate();
      emit searchBreak();
    }
    this->fstr = fstr;
    this->result.clear();
    this->isSessionReady = true;
  }
  void run() override
  {
    if (!isSessionReady)
      return;
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

  QList<QPair<int, int>> getResult() { return result; }

signals:
  void searchProgress(int);
  void searchBreak();
  void searchOver();
};

#endif // HDOCUMENTFIND_H
