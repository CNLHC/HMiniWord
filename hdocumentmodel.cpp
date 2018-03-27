#include "hdocumentmodel.h"

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
HDocumentModel::addNewLogicLine(QString Line)
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
  mLogicLine.append(newLine);
  return *newLine;
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
