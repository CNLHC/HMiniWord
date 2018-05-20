#ifndef HPAINTAREA_H
#define HPAINTAREA_H

#include "hrendercontroller.h"
#include <QPainter>
#include <QWidget>

class HPaintArea : public QWidget
{
  Q_OBJECT
public:
  /*!
 * \brief 我负责本类的构造
 * \param parent 指向父元件的指针
 * 我在构造时会在堆上申请内存以存储我的屏幕控制器。此外我在构造时会自动调整窗口大小为父亲的大小。
 */
  HPaintArea(QWidget* parent);
  /*!
    我负责本类的析构造.我在析构时会删除分配给我的屏幕控制器的内存
    */
  ~HPaintArea();
  //!父元件指针
  QWidget* mParent;
  //!屏幕控制器指针
  HRenderController* mController;
  /*!
   * \brief 组件坐标转换
   * \param point 当前组件的相对坐标
   * \return 存储着行列数的二元组
   * \todo  撰写我的边界条件及异常处理
   *
   * 我返回一个二元组，第一个对象为屏幕行，第二个对象为屏幕列.
   * 我能够根据当前滚轮的位置设置进行修正。
   */
  QPair<int, int> point2Coord(QPointF point);
  /*!
   * \brief 获取我的屏幕控制器
   * \return 指向以我的屏幕控制器的指针
   */
  HRenderController* getController() { return mController; }

private:
  /*!
   * \brief 重写父类绘图方法
   * \param event 详见QpaintEvent
   *
   * 我将由系统自动调用或者由我的父亲通过调用我继承到的 update 方法简介调用。
   * 请不要直接调用我。
   *
   * 我将根据我屏幕控制器内的数据进行绘制。
   */
  void paintEvent(QPaintEvent* event) override;
  /*!
   * \brief mVerOffset 由垂直滚轮滚动带来的修正值
   * \sa offserChange
   */
  int mVerOffset;
public slots:
  void offsetChange(int vOffset);
};

#endif // HPAINTAREA_H
