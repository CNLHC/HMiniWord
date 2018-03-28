#include "hdocumentmodel.h"

#include <QtGlobal>
HDocumentModel::HDocumentModel()
{
}

HDocumentModel::~HDocumentModel()
{
  for (auto i = mLogicLine.begin(); i != mLogicLine.end(); i++) {
    destructLogicLine(*i);
  }
}
QList<char*>&
HDocumentModel::createLogicLine(QString Line, int pos)
{
  checkStatus();
  auto newLine = constructNewLine(Line);
  mLogicLine.insert(pos, newLine);
  mCurStatus = qMakePair(pos, HLLCreated);
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
  destructLogicLine(mLogicLine[pos]);
  mLogicLine.removeAt(pos);
  mCurStatus = qMakePair(pos, HLLDeleted);
  emit modelChanged();
}

void
HDocumentModel::updateLogicLine(QString Line, int pos)
{
  checkStatus();
  mLogicLine.removeAt(pos);
  auto newLine = constructNewLine(Line);
  mLogicLine.insert(pos, newLine);
  mCurStatus = qMakePair(pos, HLLDeleted);
  emit modelChanged();
}
void
HDocumentModel::ensureStatus()
{
  mCurStatus = qMakePair(-1, HLLNoChange);
}

QSharedPointer<QString>
HDocumentModel::composeLogicLine(int row) const
{
  if (row > mLogicLine.length())
    return nullptr;
  QList<char*>& tList = *mLogicLine[row];

  QByteArray tBArray;
  for (auto i = tList.begin(); i != tList.end(); i++) {
    tBArray.append(*i);
  }
  QSharedPointer<QString> tString = QSharedPointer<QString>(new QString);
  *tString = (tString->fromUtf8(tBArray.data()));
  return tString;
}

void
HDocumentModel::checkStatus() const
{
  Q_ASSERT(mCurStatus.second == HLLNoChange); //所有操作都应该被ensure
}

QList<char*>*
HDocumentModel::constructNewLine(QString Line) const
{
  auto newLine = new QList<char*>;
  QByteArray tBArray = Line.toUtf8();
  const char* tPos = tBArray.data();
  const char* const ptrBegin = tPos;
  while (tPos - ptrBegin < tBArray.size()) {
    char* const t = new char[2];
    int i = 0;
    for (i = 0; i < 1; i++) {
      *(t + i) = *(tPos++);
    }
    *(t + i) = 0;
    newLine->append(t);
  }
  return newLine;
}

void
HDocumentModel::destructLogicLine(QList<char*>* ptrLLList)
{
  for (auto j = ptrLLList->begin(); j != ptrLLList->end(); j++) {
    delete[] * j;
  }
}
