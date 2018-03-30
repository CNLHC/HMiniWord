#ifndef HDOCUMENTMODEL_H
#define HDOCUMENTMODEL_H

#include <QList>
#include <QSharedPointer>
#include <QtGlobal>

class HDocumentModel : public QObject
{
  Q_OBJECT
public:
  /*!
 * 我负责本类的构造。
 * 我什么也不做。
 */
  HDocumentModel();
  /*!
    我负责本类的析构。我会删除分配在堆上的数据
    */
  ~HDocumentModel();
  /*!
   * \brief Model层状态枚举类型
   */
  enum ModelStatus
  { //! Model未改变
    HLLNoChange,
    //!行更新
    HLLUpdated,
    //!创建新行
    HLLCreated,
    //!行删除
    HLLDeleted
  };
  /*!
    \brief 按照题目要求存储数据
   * \param Line 要添加到该逻辑行的字符串
   * \param pos  新逻辑行将作为文档的第pos行。(从0行开始算起)
   * \return 指向新插入行的引用类型
   *
   * 添加新逻辑行，并使用要求的数据结构存储数据。
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
   * \param pos 要更新的逻辑行的行数
   *
   * 如果pos并没有指向一个合法的行数。那么我将不会更新任何数据
   *
   */
  void updateLogicLine(QString Line, int pos);
  /*!
   * \brief 删除逻辑行
   * \param pos 要删除的逻辑行的行数
   *
   * pos应当对应一个客观存在的逻辑行位置。 (0 <= pos < maxLine)
   * 如果pos不对应任何行，那么我将不删除任何东西。
   */
  void deleteLogicLine(int pos);
  /*!
   * \brief 重置 Model 层状态
   *
   * 该函数应该由Controller层在双缓冲区渲染完毕后调用.
   *
   * Model在发出 modelChanged 信号前，会根据所完成的操作设置私有变量 mCurStatus.
   * 在Cobtroller的槽接到信号后,会将Model的更改同步到C层中。
   *
   * \sa modelChanged()
   */
  void ensureStatus();
  /*!
    \brief 合成逻辑行数据
   * \param pos 逻辑行的行数
   * \return 指向QString 对象的智能指针
   *
   * 我将一个使用要求格式的逻辑行复合成一个QString类型，并返回他的智能指针。
   *
   * 如果给定的行数超出了范围,我将返回一个空指针。
   */
  QSharedPointer<QString> composeLogicLine(int row) const;
  /*!
   * \brief 获取当前Model的状态
   * \return 改变行以及改变的类型,以QPair的形式返回
   *
   */
  QPair<int, ModelStatus> getStatus() { return mCurStatus; }
  /*!
   * \brief 获取存储在model内的逻辑行的长度
   * \return 获取存储在model内的逻辑行的长度
   */
  int getLogicLineSize() { return mLogicLine.size(); }
signals:
  /*!
   * \brief 模型修改信号
   *
   * 我将被改变模型的操作发出，这样的操作包括
    * updateLogicLine(QString Line, int pos)
   *  createLogicLine(QString Line, int pos)
   * deleteLogicLine(int pos);
   *
   *在模型改变后，上述操作设置改变的状态信息
   *
   * 链接到我的slot
  * HRenderController::renderDisptach()
  * 将会读取状态信息，将模型的更新同步到Controller层，最后调用 ensurStatus()
  *清除状态
  *
  * 每次改变model前， 都会使用 checkStatus() 检查状态是否已被正确清除。
   *
   */
  void modelChanged();

private:
  /*!
   * 我是一个二维的链表，按照题目要求的格式存储数据。
   *
   */
  QList<QList<char*>*> mLogicLine;
  /*!
   * 我负责存储当前模型的改变状态.状态包括发生变动的行数,以及发生变动的种类.
   * 在新的更改操作前，我必须被清除。
   * \sa modelChanged()
   */
  QPair<int, ModelStatus> mCurStatus;
  /*!
    \brief 检查状态是否被正确清除
   *
   Model内的数据在更改后会同步到Controller中被转换成可供View层渲染的数据。C层完成转换后
   *
   应该将M层的数据置位。该函数用于在模型的C,U,D操作前调用,以保证C,M数据的一致性。
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
   * 注意我并不会删除容器。请在调用我之后手动对容器进行删除。 此处容器指
   * QList<char*>* 中的QList ，即存储堆内存的链表。
   */
  void destructLogicLine(QList<char*>* ptrLLList);
};

#endif // HDOCUMENTMODEL_H
