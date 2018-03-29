#ifndef HPAINTAREA_H
#define HPAINTAREA_H

//#include <QInputMethodEvent>
#include "hrendercontroller.h"
#include "htextcursor.h"
#include <QAbstractScrollArea>
#include <QPainter>
#include <QWidget>

class HPaintArea : public QFrame
{
  Q_OBJECT
public:
  HPaintArea(QWidget* parent, HTextCursor* cursor);
  ~HPaintArea();
  QWidget* mParent;
  HRenderController* mController;
  QPair<int, int> point2Coord(QPointF point);

private:
  void paintEvent(QPaintEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;
  HTextCursor* mCursor;
  QString a;
};

#endif // HPAINTAREA_H
