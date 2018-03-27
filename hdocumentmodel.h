#ifndef HDOCUMENTMODEL_H
#define HDOCUMENTMODEL_H

#include <QDebug>
#include <QList>
#include <QSharedPointer>
#include <QString>
#include <QtGlobal>

class HDocumentModel : QObject
{
  Q_OBJECT
public:
  HDocumentModel();
  ~HDocumentModel();

  /*!
   * \param QString 要添加的字符串
   * \return QList<char *>&
   * 添加新逻辑行.使用要求的数据结构存储数据。
   */
  QList<char*>& addNewLogicLine(QString Line);
  /*!
   * \param row 逻辑行的行数
   * \return 指向QString 对象的指针
   * 将一个逻辑行内的对象复合成一个QString类型，如果行数超出了范围,则返回一个空指针。
   */
  QSharedPointer<QString> composeLogicLine(int row) const;
signals:
  void modelChanged();

private:
  QList<QList<char*>*> mLogicLine;
};

#endif // HDOCUMENTMODEL_H
