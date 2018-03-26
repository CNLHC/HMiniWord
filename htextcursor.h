#ifndef HTEXTCURSOR_H
#define HTEXTCURSOR_H

#include <QObject>
#include <QWidget>
class HTextCursor : public QWidget
{
  Q_OBJECT
public:
  HTextCursor(QWidget* parent);
  void setPos();
  void setShape();

public slots:
  void toggleBlink();

private:
  void blink();
  void paintEvent(QPaintEvent* event) override;
  QWidget* mParent;
};

#endif // HTEXTCURSOR_H
