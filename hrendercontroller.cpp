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
  mFont = QFont("Courier", 18);
  mLineHeight = QFontMetrics(mFont).height();
  QObject::connect(this->mModel, SIGNAL(modelChanged()), this,
                   SLOT(renderDisptach()));
}
HRenderController::~HRenderController()
{
  delete mModel;
  for (int i = 0; i < mScreenLine.size(); i++) {
    delete mScreenLine.at(i);
  }
}
void
HRenderController::LineNew(int row, QString str)
{
  this->mModel->createLogicLine(str, row);
}
void
HRenderController::LineNew(QString str)
{
  int row = mModel->getLogicLineSize();
  this->mModel->createLogicLine(str, row);
}
void
HRenderController::LineDelete(int row)
{
  if (mLogicLine.length() <= 1)
    LineNew("");
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
  if (originStr->length() == 0)
    LineDelete(row);
  else
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
    case HDocumentModel::HLLUpdated:
      deRenderLogicLine(mLogicLine.begin() + row);
      renderLogicLine(mLogicLine.begin() + row);
      break;
    case HDocumentModel::HLLDeleted:
      deRenderLogicLine(mLogicLine.begin() + row);
      deleteLogicLine(mLogicLine.begin() + row);
      break;
    default:
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

  //从Model获取当前逻辑行的真实内容
  QSharedPointer<QString> tStr =
    this->mModel->composeLogicLine(iter - this->mLogicLine.begin());
  QFontMetrics fm(this->mFont);

  int screenWidth = (this->getParentSize().width() - 15 -
                     (fm.width("啊") * 1.2)); // 15是滚动条的的宽度
  auto tScreenLineList = new QList<screenLineItem>;

  //--------------从LL计算出一个SL列表:begin---------------
  int mul = 1, breakPoint = 0;
  auto genWidthList = [](QStringRef str, QFontMetrics& fm) {
    QList<int> widthList;
    for (int i = 1; i <= str.size(); i++)
      widthList.append(fm.width(str.left(i).toString()));
    return widthList;
  };
  for (int i = 1; i <= tStr->size(); i++) {
    if (fm.width(tStr->left(i)) > screenWidth * mul) {
      tScreenLineList->append(screenLineItem(
        tStr->mid(breakPoint, i - breakPoint),
        genWidthList(tStr->midRef(breakPoint, i - breakPoint), fm)));
      breakPoint = i;
      mul++;
    }
  }
  int totalSize = 0;
  for (int i = 0; i < tScreenLineList->size(); i++)
    totalSize += (*tScreenLineList)[i].mString.size();
  if (!(totalSize == tStr->size() && totalSize > 0))
    tScreenLineList->append(screenLineItem(
      tStr->mid(breakPoint, tStr->size() - breakPoint),
      genWidthList(tStr->midRef(breakPoint, tStr->size() - breakPoint), fm)));
  //--------------从LL计算出一个SL列表:end---------------

  int insertPoint = LL2LastSL(iter - 1); // 寻找上一逻辑行的最后一个屏幕行位置
  for (int i = 0; i < tScreenLineList->size(); i++)
    mScreenLine.insert(i + insertPoint + 1, &((*tScreenLineList)[i]));
  *iter = tScreenLineList; //重新为逻辑行绑定一个屏幕行
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
  for (int i = begin; i <= end; i++) {
    if (mScreenLine.at(begin) != nullptr)
      delete mScreenLine.at(begin);
    mScreenLine.removeAt(begin); //删除每一个屏幕行的数据
  }                              //!这里很容易出错啊...//!20180520更新，这里超容易出错啊！！！坑我两次
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
int
HRenderController::SL2LL(int row)
{
  for (int i = 0; i <= mLogicLine.size(); i++) {
    auto pList = mLogicLine[i];
    for (int j = 0; j < pList->size(); j++) {
      if (&(*pList)[j] == mScreenLine[row])
        return i;
    }
  }
  Q_ASSERT(1 == 0);
  return 0;
}
bool
HRenderController::isBlankLine(int row)
{
  if (mScreenLine[row]->mString.size() == 0)
    return true;
  return false;
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
int
HRenderController::SLC2LLC(int row, int column)
{
  int LLindex = SL2LL(row);
  auto heapHeader = mLogicLine[LLindex];
  int SLpos = 0;
  for (; SLpos <= heapHeader->size();
       SLpos++) //寻找给定屏幕行在其所在堆链表中的序号
    if (&((*heapHeader)[SLpos]) == mScreenLine[row])
      break;
  int offset = 0;
  for (int i = 0; i < SLpos; i++) //计算偏移量
    offset += (*heapHeader)[i].mString.size();
  return column + offset;
}
