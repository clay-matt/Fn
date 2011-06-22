//

#include <QtGui>

#include "FnGraph.h"
#include "GraphDialog.h"

/////////////////////////////////////////////////////////

GraphDialog::GraphDialog(QWidget *parent) : QDialog(parent)
{

    int i;

    graphLabel = new QLabel(tr("&Graph Name:"));
    graphLineEdit = new QLineEdit;
    graphLabel->setBuddy(graphLineEdit);

    verticesLabel = new QLabel(tr("Vertex &List:"));
    verticesTextEdit = new QTextEdit;
    verticesTextEdit->setFixedHeight(48); // this gives approx 2 lines
    verticesLabel->setBuddy(verticesTextEdit);

    edgeBoxLabel = new QLabel(tr("&Number of Edges:"));
    edgeSpinBox = new QSpinBox;
    edgeSpinBox->setMinimum(0);
    edgeSpinBox->setMaximum(Fn_MaxEdges);
    edgeSpinBox->setValue(1);
    edgeBoxLabel->setBuddy(edgeSpinBox);

    edgeGroupBox = new QGroupBox(tr("&Edge List:"));

    vertexRegExp = new QRegExpValidator;

    // edge line edits
    for (i = 0; i < Fn_MaxEdges; i++) {

        edgeNameEdit[i] = new QLineEdit;
        edgeNameEdit[i]->setPlaceholderText(tr("name"));
        edgeNameEdit[i]->setFixedWidth(75);

        initialVertexEdit[i] = new QLineEdit;
        initialVertexEdit[i]->setPlaceholderText(tr("v0"));
        initialVertexEdit[i]->setFixedWidth(75);
        initialVertexEdit[i]->setValidator(vertexRegExp);

        terminalVertexEdit[i] = new QLineEdit;
        terminalVertexEdit[i]->setPlaceholderText(tr("v1"));
        terminalVertexEdit[i]->setFixedWidth(75);
        terminalVertexEdit[i]->setValidator(vertexRegExp);

        if(i > 0) {
            edgeNameEdit[i]->hide();
            initialVertexEdit[i]->hide();
            terminalVertexEdit[i]->hide();
        }

    }

    // buttons
    cancelButton = new QPushButton(tr("Cancel"));

    loadButton = new QPushButton(tr("&Load"));
    loadButton->setDefault(true);
    loadButton->setEnabled(false);

    dismissButton = new QPushButton(tr("&Dismiss"));
    dismissButton->hide();

    // make layouts
    QHBoxLayout *graphLayout = new QHBoxLayout;
    graphLayout->addWidget(graphLabel);
    graphLayout->addWidget(graphLineEdit);
    graphLayout->addStretch();

    QHBoxLayout *vertexListLayout = new QHBoxLayout;
    vertexListLayout->addWidget(verticesLabel);
    vertexListLayout->addStretch();

    QHBoxLayout *edgeBoxLayout = new QHBoxLayout;
    edgeBoxLayout->addWidget(edgeBoxLabel);
    edgeBoxLayout->addWidget(edgeSpinBox);
    edgeBoxLayout->addStretch();

    QGridLayout *edgeLayout = new QGridLayout;
    for (i = 0; i < Fn_MaxEdges; i++) {

        edgeLayout->addWidget(edgeNameEdit[i],i,0);
        edgeLayout->addWidget(initialVertexEdit[i],i,1);
        edgeLayout->addWidget(terminalVertexEdit[i],i,2,Qt::AlignLeft);

    }

    edgeLayout->setColumnStretch(2,1);
    edgeGroupBox->setLayout(edgeLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(loadButton);
    buttonLayout->addWidget(dismissButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(graphLayout);
    mainLayout->addLayout(vertexListLayout);
    mainLayout->addWidget(verticesTextEdit);
    mainLayout->addLayout(edgeBoxLayout);
    mainLayout->addWidget(edgeGroupBox);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Load Graph"));
    setFixedHeight(sizeHint().height());

    // make connections
    connect(edgeSpinBox,SIGNAL(valueChanged(int)),
            this,SLOT(resizeEdgeList(int)));

    connect(graphLineEdit,SIGNAL(textChanged(QString)),
            this,SLOT(enableGraphLoadButton()));

    connect(verticesTextEdit,SIGNAL(textChanged()),
            this,SLOT(enableGraphLoadButton()));

    connect(verticesTextEdit,SIGNAL(textChanged()),
            this,SLOT(changeVertexRegExp()));

    for(i = 0; i < Fn_MaxEdges; i++) {

      connect(edgeNameEdit[i],SIGNAL(textChanged(QString)),
              this,SLOT(enableGraphLoadButton()));
      connect(initialVertexEdit[i],SIGNAL(textChanged(QString)),
              this,SLOT(enableGraphLoadButton()));
      connect(terminalVertexEdit[i],SIGNAL(textChanged(QString)),
              this,SLOT(enableGraphLoadButton()));

    }

    connect(cancelButton,SIGNAL(clicked()),
            this,SLOT(reject()));

    connect(loadButton,SIGNAL(clicked()),
            this,SLOT(graphLoad()));

    connect(dismissButton,SIGNAL(clicked()),
            this,SLOT(close()));

    setAttribute(Qt::WA_DeleteOnClose);

}

// PRIVATE SLOTS

void GraphDialog::changeVertexRegExp()
{

    // called when the vertex list changes, this updates the RegExp
    // Validator for initialVertexEdit and terminalVertexEdit

    QString vertexList = verticesTextEdit->toPlainText().replace(QRegExp("\\s+"),QChar('|'));
    vertexList.prepend("\\b(");
    vertexList.append(")\\b");
    vertexRegExp->setRegExp(QRegExp(vertexList));

}

void GraphDialog::enableGraphLoadButton()
{
    // called when anything changes, this checks to see is if enough data
    // is present to load the graph

    int i,nEdges = edgeSpinBox->value();
    bool enable = true;

    // test to see if there is a name for the graph
    if (graphLineEdit->text().isEmpty()) enable = false;

    // test to see if there are vertices
    if (verticesTextEdit->toPlainText().isEmpty()) enable = false;

    // test to see if there are edges
    for (i = 0; i < nEdges; i++) {

        if (edgeNameEdit[i]->text().isEmpty()) enable = false;
        if (initialVertexEdit[i]->text().isEmpty()) enable = false;
        if (terminalVertexEdit[i]->text().isEmpty()) enable = false;

    }

    loadButton->setEnabled(enable);

}

void GraphDialog::graphLoad()
{

    int i,nEdges = edgeSpinBox->value();
    FnGraph Gamma;

    QStringList vertices = verticesTextEdit->toPlainText().split(QChar(' '),QString::SkipEmptyParts);
    foreach(QString vertex, vertices) {
        Gamma.addVertex(vertex);
    }

    for(i = 0; i < nEdges; i++) {
        Gamma.addEdge(edgeNameEdit[i]->text(),initialVertexEdit[i]->text(),
                      terminalVertexEdit[i]->text());
    }

    emit updateGraphList(graphLineEdit->text(),Gamma);

    loadButton->hide();
    loadButton->setDefault(false);
    dismissButton->show();
    dismissButton->setDefault(true);

}

void GraphDialog::resizeEdgeList(int nEdges)
{

    // called when edgeSpinBox changes, this resizes the number of
    // edge line edits

    int i = 0;

    while(i < nEdges) {

        edgeNameEdit[i]->show();
        initialVertexEdit[i]->show();
        terminalVertexEdit[i]->show();
        i++;

    }

    while(i < Fn_MaxEdges) {

        edgeNameEdit[i]->hide();
        initialVertexEdit[i]->hide();
        terminalVertexEdit[i]->hide();
        i++;

    }

    edgeGroupBox->updateGeometry();
    setFixedHeight(sizeHint().height());

}
