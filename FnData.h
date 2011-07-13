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
    bool list;
    FnMap f_value;
    QList<int> i_list;
    QList<FnGraph> g_list;
    QList<FnWord> u_list;
    QList<QString> s_list;
    QString failMessage;

    QString graphOutput() const;
    QString integerOutput() const;
    QString mapOutput() const;
    QString stringOutput() const;
    QString wordOutput() const;
    QString integerListOutput() const;
    QString graphListOutput() const;
    QString wordListOutput() const;
    QString stringListOutput() const;

public:
    FnData();
    FnData(int i);
    FnData(const FnGraph &Gamma);
    FnData(const FnMap &phi);
    FnData(const FnWord &u);
    FnData(const QString &s);
    FnData(const QList<int> &integerList);
    FnData(const QList<FnGraph> &graphList);
    FnData(const QList<FnWord> &wordList);
    FnData(const QList<QString> &stringList);

    enum VariableTypes type() const { return data_type; }
    bool isList() const { return list;  }

    int integerData() const;
    FnGraph graphData() const;
    FnMap mapData() const;
    QString stringData() const;
    FnWord wordData() const;
    QList<int> integerListData() const;
    QList<FnGraph> graphListData() const;
    QList<FnWord> wordListData() const;
    QList<QString> stringListData() const;

    QString toOutput() const;

    void setInteger(int i);
    void setElement(FnWord &u);
    void setGraph(FnGraph &Gamma);
    void setMorphism(FnMap &phi);
    void setString(QString s);
    void setIntegerList(QList<int> &list);
    void setGraphList(QList<FnGraph> &list);
    void setWordList(QList<FnWord> &list);
    void setStringList(QList<QString> &list);
    void setFailMessage(QString fail);
    void addToList(const FnData &listItem);

    QList<FnData> listData() const;

    friend FnData loadGraphData(const QString & graph);
    friend FnData loadMorphismData(const QString & morphism);

};

FnData loadGraphData(const QString & graph);
FnData loadMorphismData(const QString & morphism);

typedef QHash<QString, FnData> VariableList;
typedef QHashIterator<QString, FnData> VariableListIterator;
typedef QMutableHashIterator<QString, FnData> VariableListMutableIterator;

#endif // FNDATA_H
