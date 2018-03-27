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
  mLogicLine.insert(pos, newLine);
  this->mLLCreatedFlag.insert(pos, true);
  this->mLLUpdatedFlag.insert(pos, false);
  this->mLLDeletedFlag.insert(pos, false);
  emit modelChanged();
  return *newLine;
}
QList<char*>&
HDocumentModel::createLogicLine(QString Line)
{
  return createLogicLine(Line, mLogicLine.size());
}

QSharedPointer<QString>
HDocumentModel::composeLogicLine(int row) const
{
  if (row > mLogicLine.length())
    return nullptr;
  QList<char*>& tList = *mLogicLine[row - 1];

  QSharedPointer<QString> tString = QSharedPointer<QString>(new QString);
  for (auto i = tList.begin(); i != tList.end(); i++) {
    tString->append(*i);
  }
  return tString;
}

void
HDocumentModel::deleteLogicLinek(int pos)
{
  mLogicLine.removeAt(pos);
  this->mLLCreatedFlag.removeAt(pos);
  this->mLLUpdatedFlag.removeAt(pos);
  this->mLLDeletedFlag[pos] =
    true; /*!注意，此处的状态列表长度将暂时和LogicLine不同步。将在Controller层删除渲染位置时同步*/
  emit modelChanged();
}
