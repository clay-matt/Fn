// Header file for class FnData
// union of typical data types

// author Matt Clay
// version 110525

/////////////////////////////////////////////////////////

#ifndef FNDATA_H
#define FNDATA_H

#include <QHash>
#include <QString>

#include "Fn.h"
#include "FnGraph.h"
#include "FnMap.h"
#include "FnWord.h"

class QString;

/////////////////////////////////////////////////////////

class FnData
{

public:
  FnData();
  FnData(int i);
  FnData(const FnWord &u);
  FnData(const FnMap &phi);
  FnData(const FnGraph &Gamma);

  enum VariableTypes type() const { return data_type; }

  FnGraph graphData() const;
  int integerData() const;
  FnMap mapData() const;
  FnWord wordData() const;
  QString toOutput() const;

  void setInteger(int i);
  void setElement(FnWord &u);
  void setGraph(FnGraph &Gamma);
  void setMorphism(FnMap &phi);
  void setFailMessage(QString fail);

private:

  enum VariableTypes data_type;
  FnGraph g_value;
  FnMap f_value;
  FnWord u_value;
  int i_value;
  QString failMessage;

  QString graphOutput() const;
  QString integerOutput() const;
  QString mapOutput() const;
  QString wordOutput() const;

};

typedef QHash<QString, FnData> VariableList;
typedef QHashIterator<QString, FnData> VariableListIterator;
typedef QMutableHashIterator<QString, FnData> VariableListMutableIterator;

/////////////////////////////////////////////////////////

#endif // FNDATA_H
