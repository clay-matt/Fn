// Header file for class FnData
// union of typical data types

// author Matt Clay
// version 110627

/////////////////////////////////////////////////////////

#ifndef FNDATA_H
#define FNDATA_H

#include <QHash>
#include <QList>
#include <QString>

#include "Fn.h"
#include "FnGraph.h"
#include "FnMap.h"
#include "FnWord.h"

/////////////////////////////////////////////////////////

class FnData
{

private:

    enum VariableTypes data_type;
    FnGraph g_value;
    FnMap f_value;
    FnWord u_value;
    int i_value;
    QList<FnGraph> g_list;
    QList<QString> s_list;
    QString failMessage;

    QString graphOutput() const;
    QString integerOutput() const;
    QString mapOutput() const;
    QString wordOutput() const;
    QString graphListOutput() const;
    QString stringListOutput() const;

public:
    FnData();
    FnData(int i);
    FnData(const FnGraph &Gamma);
    FnData(const FnMap &phi);
    FnData(const FnWord &u);
    FnData(const QList<FnGraph> &list);
    FnData(const QList<QString> &list);

    enum VariableTypes type() const { return data_type; }

    int integerData() const;
    FnGraph graphData() const;
    FnMap mapData() const;
    FnWord wordData() const;
    QList<FnGraph> graphListData() const;
    QList<QString> stringListData() const;

    QString toOutput() const;

    void setInteger(int i);
    void setElement(FnWord &u);
    void setGraph(FnGraph &Gamma);
    void setMorphism(FnMap &phi);
    void setGraphList(QList<FnGraph> &list);
    void setStringList(QList<QString> &list);
    void setFailMessage(QString fail);

    friend FnData loadGraphData(const QString & graph);
    friend FnData loadMorphismData(const QString & morphism);

};

FnData loadGraphData(const QString & graph);
FnData loadMorphismData(const QString & morphism);

typedef QHash<QString, FnData> VariableList;
typedef QHashIterator<QString, FnData> VariableListIterator;
typedef QMutableHashIterator<QString, FnData> VariableListMutableIterator;

#endif // FNDATA_H
