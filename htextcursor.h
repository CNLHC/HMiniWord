#ifndef HTEXTCURSOR_H
#define HTEXTCURSOR_H

#include <QObject>
#include <QWidget>
class HTextCursor : public QWidget
{
  Q_OBJECT
public:
  HTextCursor(QWidget* parent);
  void setPos(int, int);
  void setShape();

public slots:
  void toggleBlink();

private:
  void blink();
  QColor mColor1 = QColor(128, 128, 255, 128);
  QColor mColor2 = QColor(0, 0, 0, 255);
  QColor mCurColor;
  void paintEvent(QPaintEvent* event) override;
  QWidget* mParent;
  int mx;
  int my;
};

#endif // HTEXTCURSOR_H
