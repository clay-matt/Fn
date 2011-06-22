//

#include <QtGui>

#include "Fn.h"
#include "FunctionInfo.h"
#include "FunctionListWidget.h"

/////////////////////////////////////////////////////////

FunctionListWidget::FunctionListWidget(QWidget *parent)
  : QListWidget(parent)
{

  QString skeleton;
  QString statusTip;

  QHashIterator<enum FunctionNames, QString> i(presetFunctions.fcnNames());
  while (i.hasNext()) {

    i.next();
    QListWidgetItem *functionItem = new QListWidgetItem(this);
    functionItem->setText(i.value());
    skeleton = presetFunctions.fcnSkeleton(i.key());
    statusTip = presetFunctions.fcnStatusTip(i.key());
    functionItem->setStatusTip(skeleton + statusTip);
    functionItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

  }

  sortItems();
  setMouseTracking(true);

  // add connections
  connect(this, SIGNAL(itemActivated(QListWidgetItem*)),
          this, SLOT(itemSkeleton(QListWidgetItem*)));

}

/////////////////////////////////////////////////////////
// PRIVATE SLOTS

void FunctionListWidget::itemSkeleton(QListWidgetItem *item)
{

  enum FunctionNames fcnTag = presetFunctions.fcnTag(item->text());
  emit functionSkeleton(presetFunctions.fcnSkeleton(fcnTag));

}
