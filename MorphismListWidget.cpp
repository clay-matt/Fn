//
/////////////////////////////////////////////////////////

#include <QtGui>

#include "FnData.h"
#include "FnMap.h"
#include "FnWord.h"
#include "MorphismListWidget.h"

/////////////////////////////////////////////////////////

MorphismListWidget::MorphismListWidget(const VariableList &variableList,
                                       QWidget *parent) : QTableWidget(parent)
{

  // constructor of morphism table

  // make variable table
  setRowCount(0);
  setColumnCount(0);
  setColumnCount(2);

  // set headers for columns
  QTableWidgetItem *nameItem = new QTableWidgetItem;
  QTableWidgetItem *valueItem = new QTableWidgetItem;
  nameItem->setText(tr("Morphism List:"));
  valueItem->setText(tr("Images:"));
  setHorizontalHeaderItem(0,nameItem);
  setHorizontalHeaderItem(1,valueItem);

  // fill list
  updateList(variableList);

  // control geometry
  setMinimumWidth(300);
  horizontalHeader()->setResizeMode(QHeaderView::Stretch);

  // add connections
  connect(this,SIGNAL(itemActivated(QTableWidgetItem*)),
          this,SLOT(itemValue(QTableWidgetItem*)));


}

/////////////////////////////////////////////////////////
// PUBLIC SLOTS

void MorphismListWidget::updateList(const VariableList &variableList)
{

  // updates morphism list

  int row = 0;
  int rowSpan = 0;
  QTableWidgetItem *nameItem;
  QTableWidgetItem *varItem;

  clearContents();

  VariableListIterator i(variableList);
  while (i.hasNext()) {
    i.next();
    if (i.value().type() == Morphism)
      row += i.value().mapData().domainRank();
  }
  setRowCount(row);

  i.toFront();
  row = 0;
  while (i.hasNext()) {
    i.next();

    if (i.value().type() == Morphism) {
      nameItem = item(row,0);
      if (!nameItem) {
        nameItem = new QTableWidgetItem;
        setItem(row,0,nameItem);
      }
      nameItem->setText(i.key());
      nameItem->setTextAlignment(Qt::AlignLeft);
      nameItem->setTextAlignment(Qt::AlignVCenter);
      nameItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
      rowSpan = i.value().mapData().domainRank();
      setSpan(row,0,rowSpan,1);

      MapIterator x(i.value().mapData());
      while (x.hasNext()) {
        x.next();
        if (x.key().isLower()) {
          varItem = item(row,1);
          if (!varItem) {
            varItem = new QTableWidgetItem;
            setItem(row,1,varItem);
          }
          varItem->setText(QString(x.key()) + " -> " + x.value());
          varItem->setTextAlignment(Qt::AlignLeft);
          varItem->setTextAlignment(Qt::AlignVCenter);
          varItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

          row++;
        }
      }
    }
  }

}

/////////////////////////////////////////////////////////
// PRIVATE SLOTS

void MorphismListWidget::itemValue(QTableWidgetItem *item)
{

  if (item->column() == 0)
    emit echoValue(item->text() + "_");

  if (item->column() == 1)
    emit echoValue(item->text());

}

/////////////////////////////////////////////////////////
