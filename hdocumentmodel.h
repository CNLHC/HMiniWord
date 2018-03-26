#ifndef HDOCUMENTMODEL_H
#define HDOCUMENTMODEL_H

#include <QList>
#include <QString>

class HDocumentModel : QObject
{
  Q_OBJECT
public:
  HDocumentModel();
  char* addNewLine();
signals:
  void modelChanged();
};

#endif // HDOCUMENTMODEL_H
