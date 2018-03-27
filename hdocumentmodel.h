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
   * \param Line 要添加到该逻辑行的字符串
   * \param pos 新逻辑行将作为文档的第pos行。(从0行开始算起)
   * \return 指向新插入行的引用类型
   * 添加新逻辑行.使用要求的数据结构存储数据。
   * pos如果大于当前的逻辑行数，新插入的行将作为最后一行。
   * pos如果为负，新插入的行作为首行。
   */
  QList<char*>& createLogicLine(QString Line, int pos);
  /*!
   * \brief 重载类型
   * \param Line 要添加到该逻辑行的字符串
   * \return 指向新插入行的引用类型
   * 重载类型。直接在最后一行处插入
   */
  QList<char*>& createLogicLine(QString Line);
  /*!
   * \param row 逻辑行的行数
   * \return 指向QString 对象的指针
   * 将一个逻辑行内的对象复合成一个QString类型，如果行数超出了范围,则返回一个空指针。
   */
  QSharedPointer<QString> composeLogicLine(int row) const;
  /*!
   * \brief 删除逻辑行
   * \param pos 删除当前第pos行,从0开始计算。
   * pos应当对应一个客观存在的逻辑行位置。 (0 <= pos < maxLine)
   * 如果pos不对应任何行，那么将没有任何东西被删除。
   */
  void deleteLogicLinek(int pos);

signals:
  void modelChanged();

private:
  QList<QList<char*>*> mLogicLine;
  QList<bool> mLLCreatedFlag;
  QList<bool> mLLUpdatedFlag;
  QList<bool> mLLDeletedFlag;

  /*!
   * \param row 逻辑行的行数
   * \return 指向QString 对象的指针
   * Model内的数据在更改后会同步到Controller中被转换成可供View层渲染的数据。C层完成转换后
   * 应该将M层的数据置位。该函数用于在模型的C,U,D操作前调用,以保证C,M数据的一致性。
   */
  void assertFlag();
  /*!
   * \brief 数据存储的地方
   */
  void modelBackend();
};

#endif // HDOCUMENTMODEL_H
