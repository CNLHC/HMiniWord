#include "hrendercontroller.h"
#include "hdocumentmodel.h"
#include <QDebug>
#include <QObject>
#include <QtAlgorithms>
#include <QtGlobal>
HRenderController::HRenderController(QWidget* parent)
  : mParent(parent)
{
  this->mModel = new HDocumentModel();
  mFont = QFont("Arial", 29);
  QObject::connect(this->mModel, SIGNAL(modelChanged()), this,
                   SLOT(renderDisptach()));
}
HRenderController::~HRenderController()
{
  delete mModel;
}
void
HRenderController::LineNew(int row, QString str)
{
  this->mModel->createLogicLine(str, row);
}
void
HRenderController::LineDelete(int row)
{
  this->mModel->deleteLogicLine(row);
}
void
HRenderController::LineUpdateAdd(int row, int column, QString str)
{
  auto originStr = this->mModel->composeLogicLine(row);
  originStr->insert(column, str);
  this->mModel->updateLogicLine(*originStr, row);
}
void
HRenderController::LineUpdateDelete(int row, int begin, int end)
{
  Q_ASSERT(end >= begin);
  auto originStr = this->mModel->composeLogicLine(row);
  originStr->remove(begin, end - begin + 1);
  this->mModel->updateLogicLine(*originStr, row);
}
void
HRenderController::renderDisptach()
{

  renderString(0, 0);
  this->mModel->ensureStatus();
}

void
HRenderController::renderString(int begin, int end)
{
  Q_ASSERT(begin <= end);
  auto iterEnd = mLogicLine.begin() + end + 1;

  for (auto i = mLogicLine.begin() + begin; i != iterEnd; i++) {
    renderString(i);
  }
}
void
HRenderController::renderString(
  QList<QList<QPair<QString, QPoint>>*>::iterator iter)
{
  //  Q_ASSERT(iter != mLogicLine.end());
  //  delete *iter;
  QSharedPointer<QString> tStr =
    this->mModel->composeLogicLine(iter - this->mLogicLine.begin());

  auto breakPoint = renderGenBreakPoint(*tStr);
  auto screenLineList = new QList<QPair<QString, QPoint>>;

  qDebug() << breakPoint;
  for (auto m = breakPoint.begin() + 1; m != breakPoint.end(); m++) {
    auto pair = qMakePair(tStr->mid(*(m - 1), *m - *(m - 1)), QPoint(0, 0));
    screenLineList->append(qMakePair(QString("a"), QPoint(0, 0)));
  }
}

QList<int>
HRenderController::renderGenBreakPoint(const QString& line)
{
  int width = this->getParentSize().width();
  QFontMetrics fm(this->mFont);
  int mul = 1;
  QList<int> breakPoint;
  breakPoint.append(0);
  for (int i = 1; i < line.size(); i++)
    if (fm.width(line, i - 1) < width * mul &&
        fm.width(line, i) > width * mul) {
      breakPoint.append(i);
      mul++;
    }
  breakPoint.append(line.size() - 1);
  return breakPoint;
}
