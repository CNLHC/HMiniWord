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
  auto a = new HDocumentModel;
  a->addNewLogicLine(
    QString("dasjkfhfjsdklahdskaljfhlsdkjfhsldkjfsdklfasdlkfh"));
  delete a;

  //  return a.exec();
  return 0;
}
