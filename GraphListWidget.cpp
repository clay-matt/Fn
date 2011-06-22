//
/////////////////////////////////////////////////////////

#include <QtGui>

#include "FnData.h"
#include "FnGraph.h"
#include "GraphListWidget.h"

/////////////////////////////////////////////////////////

GraphListWidget::GraphListWidget(const VariableList &variableList, QWidget *parent) :
    QTableWidget(parent)
{

    // constructor of graph table

    // make variable table
    setRowCount(0);
    setColumnCount(0);
    setColumnCount(2);

    // set headers for columns
    QTableWidgetItem *nameItem = new QTableWidgetItem;
    QTableWidgetItem *valueItem = new QTableWidgetItem;
    nameItem->setText(tr("Graph List:"));
    valueItem->setText(tr("Data:"));
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

void GraphListWidget::updateList(const VariableList &variableList)
{

    // updates graph list

    int row = 0;
    int rowSpan = 0;
    QTableWidgetItem *nameItem;
    QTableWidgetItem *varItem;

    clearContents();

    VariableListIterator i(variableList);
    while (i.hasNext()) {
        i.next();
        if (i.value().type() == Graph)
        row += i.value().graphData().nEdges()+1;
    }
    setRowCount(row);

    i.toFront();
    row = 0;
    while (i.hasNext()) {
        i.next();

        if (i.value().type() == Graph) {
            nameItem = item(row,0);
            if (!nameItem) {
                nameItem = new QTableWidgetItem;
                setItem(row,0,nameItem);
            }
            nameItem->setText(i.key());
            nameItem->setTextAlignment(Qt::AlignLeft);
            nameItem->setTextAlignment(Qt::AlignVCenter);
            nameItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            rowSpan = i.value().graphData().nEdges()+1;
            setSpan(row,0,rowSpan,1);

            // display number of vertices
            varItem = item(row,1);
            if (!varItem) {
              varItem = new QTableWidgetItem;
            setItem(row,1,varItem);
            }
            varItem->setText(QString::number(i.value().graphData().nVertices()) + " Vertices");
            varItem->setTextAlignment(Qt::AlignLeft);
            varItem->setTextAlignment(Qt::AlignVCenter);
            varItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            row++;

            GraphIterator x(i.value().graphData());
            while (x.hasNext()) {
            x.next();
            varItem = item(row,1);
            if (!varItem) {
                varItem = new QTableWidgetItem;
                setItem(row,1,varItem);
            }
            varItem->setText(QString(x.key()) + " : " + x.value().at(0) + "->" + x.value().at(1));
            varItem->setTextAlignment(Qt::AlignLeft);
            varItem->setTextAlignment(Qt::AlignVCenter);
            varItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            row++;
            }
        }
    }

}

/////////////////////////////////////////////////////////
// PRIVATE SLOTS

void GraphListWidget::itemValue(QTableWidgetItem *item)
{

  if (item->column() == 0)
    emit echoValue(item->text() + "_");

  if (item->column() == 1)
    emit echoValue(item->text());

}

/////////////////////////////////////////////////////////
