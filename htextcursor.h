#ifndef HTEXTCURSOR_H
#define HTEXTCURSOR_H

#include "hrendercontroller.h"
#include <QObject>
#include <QWidget>
class HTextCursor : public QWidget
{
  Q_OBJECT
private:
  /*!
  * \brief mParent 存储父类指针.
  */
  QWidget* mParent;
  /*!
   * \brief mController 存储聚合的 HRenderController 对象指针。
   */
  HRenderController* mController;
  /*!
   * \brief BlinkTimer 用于控制闪烁的定时器。
   */
  QTimer* BlinkTimer;
  /*!
   * \brief mColor1 游标闪烁颜色1
   */
  QColor mColor1 = QColor(128, 128, 255, 128);
  /*!
   * \brief mColor2 游标闪烁颜色2
   */
  QColor mColor2 = QColor(128, 128, 255, 0);
  /*!
   * \brief mColor2 表示当前游标的显示颜色,可能是mColor1 或是 mColor2.
   */
  QColor mCurColor;
  /*!
   * \brief renderAera
   * 存储了当前有哪些地方需要被渲染。每个需要显示的行对应一个QRect.
   */
  QList<QRect> renderAera;
  /*!
   * \brief priCursor 存储主游标的位置.主游标是靠后的游标。
   */
  QPair<int, int> priCursor;
  /*!
   * \brief priCursor 存储副游标的位置.副游标是靠前的游标。
   */
  QPair<int, int> altCursor;
  /*!
   * \brief blink 闪烁游标
   * 调用本函数将启动一个定时器中断。在该定时器移除后将回调 toggleBlink()
   * 槽.该定时器会自动重置
   */
  void blink();
  /*!
   * \brief 关闭由调用 blink() 而启动的定时器。
   */
  void Closeblink();
  /*!
   * \brief 重载paintEvent 将游标绘制在屏幕上.
   * \param event 默认参数
   */
  void paintEvent(QPaintEvent* event) override;
  /*!
   * \brief 将靠后的游标设置为主游标
   * \param row1 游标1的行数
   * \param column1 游标1的列数
   * \param row2 游标2的行数
   * \param column2 游标2的列数
   * \todo 解释双游标的设计，并在此处链接该解释
   */
  void setCursor(int row1, int column1, int row2, int column2);
  /*!
   * \brief mVerOffset 由垂直滚轮滚动带来的修正值
   */
  int mVerOffset;
  bool cursorOverflowflag;

public:
  enum MoveDirection
  {
    cursorUp,
    cursorDown,
    cursorLeft,
    cursorRight,
  };
  /*!
   * \brief 我负责HTextCursor的构造
   * \param parent  父对象指针
   * \param controller HRenderController 对象指针
   */
  HTextCursor(QWidget* parent, HRenderController* controller);
  /*!
   * \brief 设置游标位置
   * \param row1 游标1逻辑行数
   * \param column1 游标1列数
   * \param row2 游标2逻辑行数
   * \param column2 游标2列数
   * 我会根据给定的行和列，计算出需要显示游标的位置，更新 renderArea, 然后通过
   * update()
   * 函数调用 paintEvent() 实现游标显示的更新。
   */
  void setPos(int row1, int column1, int row2, int column2);
  /*!
   * \brief setPos(int row1, int column1, int row2, int column2)
   * 的重载类型，用于两个游标位置相同时的游标设置。
   * \param row1
   * \param column1
   */
  void setPos(int row1, int column1);
  /*!
   * \brief getPriCursor 获取主游标 priCursor 的位置信息
   * \return  QPair,代表主游标所在的行与列
   */
  QPair<int, int> getPriCursor() { return priCursor; }
  /*!
   * \brief getPriCursor 获取副游标  altCursor 的位置信息
   * \return  QPair,代表副游标所在的行与列
   */
  QPair<int, int> getAltCursor() { return altCursor; }
  /*!
   * \brief 移动光标的位置
   * \param dir MoveDirection 枚举类型
   * 注意只有存在一个光标 altCursor 和 priCursor 时，才会移动光标
   */
  void move(MoveDirection dir);

signals:
  void cursorOverflow();

public slots:
  /*!
   * \brief toggleBlink 切换当前游标的显示颜色，实现闪烁的效果。
   */
  void toggleBlink();
  void verScrollBarMove(int offset);
};

#endif // HTEXTCURSOR_H
