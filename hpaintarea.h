#ifndef HPAINTAREA_H
#define HPAINTAREA_H

//#include <QInputMethodEvent>
#include "htextcursor.h"
#include <QAbstractScrollArea>
#include <QPainter>
#include <QWidget>

class HPaintArea : public QFrame
{
  Q_OBJECT
public:
  HPaintArea(QWidget* parent = 0);
  void drawOutline(QPainter& painter)
  {
    painter.setPen(Qt::darkGreen);
    painter.setPen(Qt::DashLine);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(0, 0, 50, 50);
  }
  ~HPaintArea();

private:
  void paintEvent(QPaintEvent* event) override;
  void inputMethodEvent(QInputMethodEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;
  QRect xBoundingRect;
  QRect yBoundingRect;
  HTextCursor* mCursor;
  QString a;
};

#endif // HPAINTAREA_H
