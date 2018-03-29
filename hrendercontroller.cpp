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
  mFont = QFont("Arial", 18);
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
  if (originStr->size() == 0) {
    LineNew(row, str);
    return;
  }
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
  int row = this->mModel->getStatus().first;
  HDocumentModel::ModelStatus status = this->mModel->getStatus().second;

  switch (status) {
    case HDocumentModel::HLLCreated:
      createLogicLine(row);
      renderLogicLine(mLogicLine.begin() + row);
      break;
    default:
    case HDocumentModel::HLLUpdated:
      deRenderLogicLine(mLogicLine.begin() + row);
      renderLogicLine(mLogicLine.begin() + row);
      break;
  }

  determinRenderPosition();
  this->mParent->update();
  this->mModel->ensureStatus();
}

void
HRenderController::createLogicLine(int pos)
{
  mLogicLine.insert(pos, nullptr);
}
void
HRenderController::renderLogicLine(QList<QList<screenLineItem>*>::iterator iter)
{
  Q_ASSERT(iter != mLogicLine.end()); //只有存在的LogicLine才能被渲染
  Q_ASSERT(*iter == nullptr);         //只有空的LogicLine才能被渲染
  QSharedPointer<QString> tStr =
    this->mModel->composeLogicLine(iter - this->mLogicLine.begin());
  QFontMetrics fm(this->mFont);
  int screenWidth = (this->getParentSize().width() - 15 -
                     (fm.width("啊") * 1.2)); // 40是滚动条的的宽度
  auto screenLineList = new QList<screenLineItem>;
  int mul = 1, breakPoint = 0;
  auto genWidthList = [](QStringRef str, QFontMetrics& fm) {
    QList<int> widthList;
    for (int i = 1; i <= str.size(); i++)
      widthList.append(fm.width(str.left(i).toString()));
    return widthList;
  };

  for (int i = 1; i <= tStr->size(); i++) {
    if (fm.width(tStr->left(i)) > screenWidth * mul) {
      screenLineList->append(screenLineItem(
        tStr->mid(breakPoint, i - breakPoint),
        genWidthList(tStr->midRef(breakPoint, i - breakPoint), fm)));
      breakPoint = i;
      mul++;
    }
  }
  screenLineList->append(screenLineItem(
    tStr->mid(breakPoint, tStr->size() - breakPoint),
    genWidthList(tStr->midRef(breakPoint, tStr->size() - breakPoint), fm)));

  int insertPoint = LL2LastSL(iter - 1);

  for (int i = 0; i < screenLineList->size(); i++)
    mScreenLine.insert(i + insertPoint + 1, &((*screenLineList)[i]));

  *iter = screenLineList;
}
void
HRenderController::deleteLogicLine(QList<QList<screenLineItem>*>::iterator iter)
{
  Q_ASSERT(*iter == nullptr);
  mLogicLine.removeAt(iter - mLogicLine.begin());
}
void
HRenderController::deRenderLogicLine(
  QList<QList<screenLineItem>*>::iterator iter)
{
  Q_ASSERT(iter != mLogicLine.end());
  Q_ASSERT(*iter != nullptr);
  int begin = LL2FirstSL(iter);
  int end = LL2LastSL(iter);
  for (int i = 0; i <= end; i++) //!这里很容易出错啊...
    mScreenLine.removeAt(begin);
  delete *iter;
  *iter = nullptr;
}

int
HRenderController::LL2LastSL(QList<QList<screenLineItem>*>::iterator iter)
{
  int context = 0;
  if (iter - mLogicLine.begin() < 0)
    return -1;
  context = mScreenLine.indexOf(&((*iter)->last()));
  return context;
}
int
HRenderController::LL2FirstSL(QList<QList<screenLineItem>*>::iterator iter)
{
  int context = 0;
  if (iter - mLogicLine.begin() < 0)
    return -1;
  context = mScreenLine.indexOf(&((*iter)->first()));
  return context;
}

void
HRenderController::determinRenderPosition()
{

  QFontMetrics fm(this->mFont);
  int height = 0;
  for (auto i = mScreenLine.begin(); i != mScreenLine.end(); i++) {
    height =
      (i - mScreenLine.begin() + 1) * (fm.height() + this->mLineinterval);
    (**i).mRenderPos = QPoint(xLeftOffset, height);
  }
  if (height > mParent->size().height()) {
    maxHeight = height;
    emit lineExceed();
  }
}
