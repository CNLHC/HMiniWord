#ifndef HRENDERCONTROLLER_H
#define HRENDERCONTROLLER_H

#include "hdocumentmodel.h"
#include <QObject>
#include <QWidget>

class HRenderController : public QObject
{
  Q_OBJECT
public:
  HRenderController(QWidget* parent, HDocumentModel* model)
    : mParent(parent)
    , mModel(model)
  {
  }
  QSize getParentSize() { return this->mParent->size(); }
  void render();

private:
  QWidget* mParent;
  HDocumentModel* mModel;
};

#endif // HRENDERCONTROLLER_H
