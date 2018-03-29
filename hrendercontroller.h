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
  QList<QList<screenLineItem>*> mLogicLine;
  HDocumentModel* mModel;

  void createLogicLine(int pos);
  void renderLogicLine(QList<QList<screenLineItem>*>::iterator iter);
  void deleteLogicLine(QList<QList<screenLineItem>*>::iterator iter);
  void deRenderLogicLine(QList<QList<screenLineItem>*>::iterator iter);
  int LL2LastSL(QList<QList<screenLineItem>*>::iterator iter);
  int LL2FirstSL(QList<QList<screenLineItem>*>::iterator iter);
  void determinRenderPosition();

signals:
  void lineExceed();

public slots:
  void renderDisptach();

public:
  QFont mFont;
  HRenderController(QWidget* parent);
  int maxHeight;
  int mLineinterval = 0;
  int xLeftOffset = 5;

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
  QList<screenLineItem*> mScreenLine;
};

#endif // HRENDERCONTROLLER_H
