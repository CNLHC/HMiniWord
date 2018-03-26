#include "htextcursor.h"
#include "mainwindow.h"
#include <QApplication>

int
main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  HTextCursor b(0);

  return a.exec();
}
