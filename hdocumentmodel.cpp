#include "hdocumentmodel.h"

#include <QtGlobal>
HDocumentModel::HDocumentModel()
{
}

HDocumentModel::~HDocumentModel()
{
  for (auto i = mLogicLine.begin(); i != mLogicLine.end(); i++) {
    for (auto j = (*i)->begin(); j != (*i)->end(); j++) {
      delete[] * j;
    }
  }
}
QList<char*>&
HDocumentModel::createLogicLine(QString Line, int pos)
{
  checkStatus();
  auto newLine = constructNewLine(Line);
  mLogicLine.insert(pos, newLine);
  mLLStatus.insert(pos, HLLCreated); //更新状态
  emit modelChanged();
  return *newLine;
}

QList<char*>&
HDocumentModel::createLogicLine(QString Line)
{
  return createLogicLine(Line, mLogicLine.size());
}

void
HDocumentModel::deleteLogicLine(int pos)
{
  checkStatus();
  mLogicLine.removeAt(pos);
  mLLStatus[pos] = HLLDeleted;
  emit modelChanged();
}

void
HDocumentModel::updateLogicLine(QString Line, int pos)
{
  checkStatus();
  mLogicLine.removeAt(pos);
  auto newLine = constructNewLine(Line);
  mLogicLine.insert(pos, newLine);
  mLLStatus[pos] = HLLUpdated;
  emit modelChanged();
}
void
HDocumentModel::ensureStatus(int pos, ModelStatus type)
{
  switch (type) {
    case HLLCreated:
      mLLStatus[pos] = HLLNoChange;
      break;
    case HLLDeleted:
      mLLStatus.removeAt(pos);
      break;
    case HLLUpdated:
      mLLStatus[pos] = HLLNoChange;
      break;
    default:
      break;
  }
}

QSharedPointer<QString>
HDocumentModel::composeLogicLine(int row) const
{
  if (row > mLogicLine.length())
    return nullptr;
  QList<char*>& tList = *mLogicLine[row];

  QSharedPointer<QString> tString = QSharedPointer<QString>(new QString);
  for (auto i = tList.begin(); i != tList.end(); i++) {
    tString->append(*i);
  }
  return tString;
}

void
HDocumentModel::checkStatus() const
{
  for (auto i = mLLStatus.begin(); i != mLLStatus.end(); i++) {
    Q_ASSERT(*i != HLLCreated); //所有操作都应该被ensure
    Q_ASSERT(*i != HLLUpdated); //所有操作都应该被ensure
    Q_ASSERT(*i != HLLDeleted); //所有操作都应该被ensure
  }
}

QList<char*>*
HDocumentModel::constructNewLine(QString Line) const
{
  auto newLine = new QList<char*>;
  QByteArray t = Line.toUtf8();
  const char* tPos = t.data();
  while (*tPos != 0) {
    char* const t = new char[10];
    int i = 0;
    for (i = 0; i < 9 && tPos != 0; i++)
      *(t + i) = *(tPos++);
    *(t + i) = 0;
    newLine->append(t);
  }
  return newLine;
}
