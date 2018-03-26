#ifndef HPAINTAREA_H
#define HPAINTAREA_H

//#include <QInputMethodEvent>
#include <QAbstractScrollArea>
#include <QPainter>
#include <QWidget>

class HPaintArea : public QWidget
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

private:
  void paintEvent(QPaintEvent* event) override;
  void inputMethodEvent(QInputMethodEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  QRect xBoundingRect;
  QRect yBoundingRect;
};

#endif // HPAINTAREA_H
