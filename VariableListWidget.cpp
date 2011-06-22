//

#include <QtGui>

//#include "FnData.h"
//#include "FnWord.h"
#include "VariableListWidget.h"

/////////////////////////////////////////////////////////

VariableListWidget::VariableListWidget(const VariableList & variableList,
			       QWidget *parent) 
  : QTableWidget(parent)
{

  // constructor of variable table

  // make variable table
  setRowCount(0);
  setColumnCount(0);
  setColumnCount(2);

  // set headers for columns
  QTableWidgetItem *nameItem = new QTableWidgetItem;
  QTableWidgetItem *varItem = new QTableWidgetItem;
  nameItem->setText(tr("Variable List:"));
  varItem->setText(tr("Value:"));
  setHorizontalHeaderItem(0,nameItem);
  setHorizontalHeaderItem(1,varItem);
  setSortingEnabled(false);

  // control geometry
  setMinimumWidth(300);
  horizontalHeader()->setResizeMode(QHeaderView::Stretch);

  // fill list
  updateList(variableList);

  // add connections
  connect(this,SIGNAL(itemActivated(QTableWidgetItem*)),
          this,SLOT(itemValue(QTableWidgetItem*)));
  
}

/////////////////////////////////////////////////////////
// PUBLIC SLOTS

void VariableListWidget::updateList(const VariableList & variableList)
{

  // updates variable dialog

  int row = 0;
  QTableWidgetItem *nameItem;
  QTableWidgetItem *varItem;

  clearContents();

  VariableListIterator i(variableList);
  while (i.hasNext()) {
    i.next();
    if (i.value().type() == Element || i.value().type() == Integer)
      row++;
  }
  setRowCount(row);

  i.toFront();
  row = 0;
  while (i.hasNext()) {
    i.next();

    if (i.value().type() == Element || i.value().type() == Integer) {
      nameItem = item(row,0);
      if (!nameItem) {
        nameItem = new QTableWidgetItem;
        setItem(row,0,nameItem);
      }
      nameItem->setText(i.key());
      nameItem->setTextAlignment(Qt::AlignLeft);
      nameItem->setTextAlignment(Qt::AlignVCenter);
      nameItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

      varItem = item(row,1);
      if (!varItem) {
        varItem = new QTableWidgetItem;
        setItem(row,1,varItem);
      }
      varItem->setText(i.value().toOutput());
      varItem->setTextAlignment(Qt::AlignLeft);
      varItem->setTextAlignment(Qt::AlignVCenter);
      varItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

      row++;
    }

  }

}

/////////////////////////////////////////////////////////
// PRIVATE SLOTS

void VariableListWidget::itemValue(QTableWidgetItem *item)
{

  if (item->column() == 0)
    emit echoValue(item->text() + "_");

  if (item->column() == 1)
    emit echoValue(item->text());

}
