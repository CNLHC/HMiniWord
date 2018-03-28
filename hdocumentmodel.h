#ifndef HDOCUMENTMODEL_H
#define HDOCUMENTMODEL_H

#include <QDebug>
#include <QList>
#include <QSharedPointer>
#include <QString>
#include <QtGlobal>

class HDocumentModel : public QObject
{
  Q_OBJECT
public:
  HDocumentModel();
  ~HDocumentModel();
  /*!
   * \brief Model层状态枚举类型
   *
   */
  enum ModelStatus
  {
    HLLNoChange,
    HLLUpdated,
    HLLCreated,
    HLLDeleted
  };

  /*!
   * \param Line 要添加到该逻辑行的字符串
   * \param pos 新逻辑行将作为文档的第pos行。(从0行开始算起)
   * \return 指向新插入行的引用类型
   *
   * 添加新逻辑行.使用要求的数据结构存储数据。
   * pos如果大于当前的逻辑行数，新插入的行将作为最后一行。
   * pos如果为负，新插入的行作为首行。
   */
  QList<char*>& createLogicLine(QString Line, int pos);
  /*!
   * \brief 重载类型
   * \param Line 要添加到该逻辑行的字符串
   * \return 指向新插入行的引用类型
   *
   * 我是为 createLogicLine(QString,int)
   * 的重载类型。使用我直接在最后一行处插入Logic Line
   */
  QList<char*>& createLogicLine(QString Line);
  /*!
   * \brief 更新逻辑行
   * \param Line 新的逻辑行字符串
   * \param pos 要更新的位置
   *
   * 如果pos并没有指向一个合法的行数。那么我将不会更新任何数据
   */
  void updateLogicLine(QString Line, int pos);
  /*!
   * \brief 删除逻辑行
   * \param pos 删除当前第pos行,从0开始计算。
   *
   * pos应当对应一个客观存在的逻辑行位置。 (0 <= pos < maxLine)
   * 如果pos不对应任何行，那么将没有任何东西被删除。
   */
  void deleteLogicLine(int pos);
  /*!
   * \brief 重置Model层状态
   * \param pos 需要重置状态的逻辑行数
   * \param type 重置状态的类型 参见 #ModelStatus.
   *
   * 该函数应该由Controller层在双缓冲区渲染完毕后调用.
   */
  void ensureStatus();
  /*!
   * \param pos 逻辑行的行数
   * \return 指向QString 对象的指针
   *
   * 我将一个逻辑行内的对象复合成一个QString类型，并返回他的指针。如果行数超出了范围,则返回一个空指针。
   */
  QSharedPointer<QString> composeLogicLine(int row) const;
  /*!
   * \brief 获取当前Model的状态
   */
  ModelStatus getStatus() { return mCurStatus.second; }
  /*!
   * \brief 获取当前的逻辑行长度
   */
  int getLogicLineSize() { return mLogicLine.size(); }

signals:
  void modelChanged();

private:
  QList<QList<char*>*> mLogicLine;

  QList<ModelStatus> mLLStatus;
  QPair<int, ModelStatus> mCurStatus;

  /*!
   * Model内的数据在更改后会同步到Controller中被转换成可供View层渲染的数据。C层完成转换后
   * 应该将M层的数据置位。该函数用于在模型的C,U,D操作前调用,以保证C,M数据的一致性。
   */
  void checkStatus() const;
  /*!
   * \brief 按照题目要求，构造一个存储逻辑行数据结构
   * \param Line 要构造的逻辑行的字符串
   * \return 一个由100字节为单位的char类型组成的链表
   *
   */
  QList<char*>* constructNewLine(QString Line) const;
  /*!
   * \brief 释放逻辑行占用的堆内存
   * \param ptrLList 指向存储堆结构的链表头指针
   *
   * 注意我并不会删除容器。请在调用我之后手动对容器进行删除。
   */
  void destructLogicLine(QList<char*>* ptrLLList);
};

#endif // HDOCUMENTMODEL_H
