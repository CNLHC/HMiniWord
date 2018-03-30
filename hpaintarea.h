#ifndef HPAINTAREA_H
#define HPAINTAREA_H

//#include <QInputMethodEvent>
#include "hrendercontroller.h"
#include "htextcursor.h"
#include <QAbstractScrollArea>
#include <QPainter>
#include <QWidget>

class HPaintArea : public QWidget
{
  Q_OBJECT
public:
  HPaintArea(QWidget* parent);
  ~HPaintArea();
  QWidget* mParent;
  QPair<int, int> point2Coord(QPointF point);
  HRenderController* getController() { return mController; }
  HRenderController* mController;

private:
  void paintEvent(QPaintEvent* event) override;
  QString a;
};

#endif // HPAINTAREA_H
