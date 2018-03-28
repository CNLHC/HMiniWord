#ifndef HRENDERCONTROLLER_H
#define HRENDERCONTROLLER_H

#include "hdocumentmodel.h"
#include <QObject>
#include <QWidget>

class HRenderController : public QObject
{
  Q_OBJECT
private:
  QWidget* mParent;

  QList<QList<QPair<QString, QPoint>>*> mLogicLine;
  HDocumentModel* mModel;
  QFont mFont;
  /*!
   * \brief  逻辑行渲染重载
   * \param begin 逻辑行开始点
   * \param end 逻辑行结束点
   *
   * 我是 renderString(QList<QList<QString*>>::iterator iter) 的重载类型.
   * 我可以对一个范围内的逻辑行进行渲染。
   *
   */
  void renderString(int begin, int end);
  /*!
   * \brief 将逻辑行渲染为屏幕行
   * \param iter 指向需要渲染逻辑行的迭代器
   * \todo 可以将线性查找优化为二分查找
   *
   * 我的主要任务是将一个逻辑行渲染成若干个屏幕行，以将它们完整的显示在屏幕上。
   *
   * 我将直接在 mScreenLine 变量上作修改.
   */ void
  renderString(QList<QList<QPair<QString, QPoint>>*>::iterator iter);
  void renderPoistionTable();

  /*!
   * \brief  计算换行点
   * \param line 输入字符串.
   * \return 换行点列表
   *
   * 我负责计算在当前的屏幕宽度下，输入字符串应该在哪些位置换行才能保证文本不被遮挡。
   *
   * 我返回的换行点列表至少包括两个值。(在不换行的情况下，返回首字符位置和尾字符位置)
   */
  QList<int> renderGenBreakPoint(const QString& line);

public slots:
  void renderDisptach();

public:
  HRenderController(QWidget* parent);
  ~HRenderController();
  /*!
   * \brief 获取父元素尺寸
   */
  QSize getParentSize() { return this->mParent->size(); }
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

  /*!
   * 我负责存储显示屏幕行所需要的所有信息
   * 其中Qpair的第一个变量 为渲染的位置.
   * 第二个变量为该行文本
   */
  QList<QPair<QString, QPoint>*> mScreenLine;
};

#endif // HRENDERCONTROLLER_H
