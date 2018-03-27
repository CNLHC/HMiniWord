#include "hdocumentmodel.h"
#include "htextcursor.h"
#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QList>

int
main(int argc, char* argv[])
{
  //  QApplication a(argc, argv);
  //  MainWindow w;
  //  w.show();
  //  auto a = new HDocumentModel;
  //  a->addNewLogicLine(
  //    QString("dasjkfhfjsdklahdskaljfhlsdkjfhsldkjfsdklfasdlkfh"));
  //  delete a;
  QList<int> a;
  a.insert(-1, 1);
  a.insert(0, 2);
  a.insert(5, 3);
  qDebug() << a;
  qDebug() << a;

  //  return a.exec();
  return 0;
}
