#ifndef HRENDERCONTROLLER_H
#define HRENDERCONTROLLER_H

#include "hdocumentmodel.h"
#include <QObject>
#include <QWidget>

class HRenderController : public QObject
{
  Q_OBJECT
private:
  struct screenLineItem
  {
    screenLineItem(QString str, QList<int> widthList)
      : mString(str)
      , mWidthList(widthList)
    {
    }
    QString mString;
    QList<int> mWidthList;
    QPoint mRenderPos;
  };
  QWidget* mParent;
  HDocumentModel* mModel;
  void createLogicLine(int pos);
  /*!
   * \brief renderLogicLine 重新渲染当前逻辑行
   * \param iter 指向需要渲染的逻辑行的迭代器
   * 在调用我前，请确保逻辑行已经被deRender.即 *iter 必须指向一个空指针
   */
  void renderLogicLine(QList<QList<screenLineItem>*>::iterator iter);
  /*!
   * \brief renderLogicLine 删除一个逻辑行对应的数据
   * \param iter 指向需要删除的逻辑行的迭代器
   *
   */
  void deleteLogicLine(QList<QList<screenLineItem>*>::iterator iter);
  void deRenderLogicLine(QList<QList<screenLineItem>*>::iterator iter);
  void determinRenderPosition();
  /*!
   * \brief 获取父元素尺寸
   */
  QSize getParentSize() { return this->mParent->size(); }

signals:
  void lineExceed();

public slots:
  void renderDisptach();

public:
  QFont mFont;
  HRenderController(QWidget* parent);
  /*!
    我负责本类的析构,目前我什么也不做
    \todo 编写析构函数
    */
  ~HRenderController();
  /*!
   * \brief mLogicLine 存储当前的逻辑行相关信息
   * mLogicLine 是一个链表, 链表中的每一个元素都是 **另一个QList
   * 中某一个元素的的地址**。
   * 另一个 QList 指的是 mScreenLine 。
   * 该地址是某逻辑行对应的若干屏幕行的头地址。
   * 也就是说， mLogicLine实际上不存储任何数据，只存储在数据的地址。
   * 所有的数据都是手工分配到堆上的。
   */
  QList<QList<screenLineItem>*> mLogicLine;
  /*!
   * \brief mScreenLine 存储屏幕行相关信息.
   * 我的每一个元素都是一个 screenLineItem 对象的指针,对应屏幕中的一行。
   */
  QList<screenLineItem*> mScreenLine;
  /*!
   * \brief SL2LL 给定一屏幕行行数，返回该屏幕行对应的逻辑行
   * \param row 屏幕行绝对行数
   * \return 逻辑行 mLogicLine 的索引
   * \sa SLC2LLC
   * \todo 1. 时间复杂度过高 2. 最好能够返回迭代器而非索引
   */
  int SL2LL(int row);
  /*!
   * \brief SLC2LLC 给定一屏幕行的行数和列数，返回真实的列数
   * \param row 屏幕行的绝对行数
   * \param column 当前屏幕行的列数
   * \return 该内容在逻辑行中的列数
   * \sa SL2LL
   */
  int SLC2LLC(int row, int column);
  /*!
   * \brief LL2LastSL
   * 给定一逻辑行的迭代器，返回该逻辑行对应的最后一个屏幕行的绝对行数
   * \param iter mLogicLine 容器的迭代器
   * \return 屏幕行绝对行数
   * \sa  LL2FirstSL
   */
  int LL2LastSL(QList<QList<screenLineItem>*>::iterator iter);
  /*!
   * \brief LL2LastSL
   * 给定一逻辑行的迭代器，返回该逻辑行对应的第一个屏幕行的绝对行数
   * \param iter mLogicLine 容器的迭代器
   * \return 屏幕行绝对行数
   * \sa  LL2LastSL
   */
  int LL2FirstSL(QList<QList<screenLineItem>*>::iterator iter);
  /*!
   * \brief isBlankLine 检查某一屏幕行是否是空行
   * \param row 屏幕行的绝对行数
   * \return 是空行-True 不是空行False
   */
  bool isBlankLine(int row);

  int maxHeight;
  int mLineinterval = 0;
  int xLeftOffset = 5;
  int mLineHeight;
  /*!
     * \brief 更新操作(插入)
     * \param row 屏幕行的行数
     * \param column 插入新元素的位置
     * \param str 插入的字符串
     */
  void LineUpdateAdd(int row, int column, QString str);
  /*!
     * \brief 更新操作(删除)
     * \param row 屏幕行的行数
     * \param begin 删除开始位置
     * \param end  删除结束位置
     */
  void LineUpdateDelete(int row, int begin, int end);
  /*!
     * \brief 删除行操作
     * \param row 要删除行的行数
     */
  void LineDelete(int row);
  /*!
     * \brief 创建新行操作
     * \param row 新行的行数
     * \param str 要插入的字符串
     */
  void LineNew(int row, QString str);
  /*!
     * \brief 由屏幕行的行数得到逻辑行的行数
     * \param row 屏幕行的行数
     */
  int SL2LL(int row) const;
};

#endif // HRENDERCONTROLLER_H
