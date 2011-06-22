// Header file for class GraphDialog
// inherits QDialog

// author Matt Clay
// version 110608

/////////////////////////////////////////////////////////

#ifndef GRAPHDIALOG_H
#define GRAPHDIALOG_H

#include <QDialog>

#include "Fn.h"
#include "FnGraph.h"

/////////////////////////////////////////////////////////

class QGroupBox;
class QLabel;
class QLineEdit;
class QRegExpValidator;
class QPushButton;
class QScrollArea;
class QSpinBox;
class QTextEdit;

class GraphDialog : public QDialog
{

    Q_OBJECT

public:
    GraphDialog(QWidget *parent = 0);

private:
    QLabel *graphLabel;
    QLineEdit *graphLineEdit;
    QLabel *verticesLabel;
    QTextEdit *verticesTextEdit;
    QLabel *edgeBoxLabel;
    QSpinBox *edgeSpinBox;
    QPushButton *cancelButton;
    QPushButton *loadButton;
    QPushButton *dismissButton;

    QRegExpValidator *vertexRegExp;

    QLineEdit *edgeNameEdit[Fn_MaxEdges];
    QLineEdit *initialVertexEdit[Fn_MaxEdges];
    QLineEdit *terminalVertexEdit[Fn_MaxEdges];
    QGroupBox *edgeGroupBox;

signals:
    void updateGraphList(const QString &name, const FnGraph &Gamma);

public slots:

private slots:
    void changeVertexRegExp();
    void enableGraphLoadButton();
    void graphLoad();
    void resizeEdgeList(int nEdges);

};

#endif // GRAPHDIALOG_H
