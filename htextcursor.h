#ifndef HTEXTCURSOR_H
#define HTEXTCURSOR_H

#include <QObject>
#include <QWidget>
class HTextCursor : public QObject
{
  Q_OBJECT
public:
  HTextCursor(QWidget* parent)
    : mParent(parent)
  {
    blink();
  }
  void setPos();
  void setShape();

public slots:
  void toggleBlink();

private:
  void blink();

  QWidget* mParent;
};

#endif // HTEXTCURSOR_H
