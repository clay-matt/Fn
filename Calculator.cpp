//
/////////////////////////////////////////////////////////

#include <QChar>
#include <QHash>
#include <QMessageBox>
#include <QRegExp>
#include <QString>

#include "Calculator.h"

const QString Calculator::IdString = "Id";
const QString Calculator::PreviousOutput = "_";

/////////////////////////////////////////////////////////

Calculator::Calculator(QObject *parent) : QObject(parent)
{

    basis.changeRank(Fn_MaxRank);
    previousOutput.setFailMessage(tr("Name Error: \"%1\" is not defined").arg(PreviousOutput));

    // make regular expressions
    // matches string = anything
    assignmentRegExp.setPattern(QString("^\\b\\S+\\b\\s*=.+$"));

    // matches ( anything )
    graphRegExp.setPattern(QString("^\\(.+\\)$"));

    // matches [ anything ]
    listRegExp.setPattern(QString("^\\[.+\\]$"));

    // matches string_(anything)
    functionRegExp.setPattern(QString("^\\S+_\\(.+\\)$"));

    // matches { anything }
    morphismRegExp.setPattern(QString("^\\{.+\\}$"));

    // matches string without equal sign
    stringRegExp.setPattern(QString("^[^=]+$"));
  
    // matches string_
    variableRegExp.setPattern(QString("^\\S+_$"));

    // matches word with only characters and digits
    wordRegExp.setPattern(QString("^\\w+$"));

    // place identity into variable list
    variableList.insert(IdString,FnData(Id));

}

/////////////////////////////////////////////////////////
// PUBLIC

QString Calculator::processInput(const QString & user_input)
{

  // processes user input
  basis.changeRank(Fn_MaxRank);

  // remove extra white space
  QString input = user_input.trimmed();

  // check previous output
  if (input == PreviousOutput)
    return previousOutput.toOutput();

  // echo after processing input
  if (stringRegExp.exactMatch(input)) {

    FnData output = compute(input);
    previousOutput = output;

    return output.toOutput();

  }

  // assignment processed input to variable
  if (assignmentRegExp.exactMatch(input)) {

    // varName = anything to left of first equal sign
    QString varName = input.section(QChar('='),0,0).trimmed();

    // check that varName is an appropriate name for a variable
    if (varName == Id) {
      previousOutput.setFailMessage(tr("Name Error: %1 is a protected character").arg(IdString));
      return previousOutput.toOutput();
    }
    if (varName == PreviousOutput){
      previousOutput.setFailMessage(tr("Name Error: %1 is a protected character").arg(PreviousOutput));
      return previousOutput.toOutput();
    }
    if (!varName.contains(QRegExp("[A-Za-z]"))) {
      previousOutput.setFailMessage(tr("Name Error: variable names must contain a letter"));
      return previousOutput.toOutput();
    }

    // varValue = everything to the right of the first equal sign
    QString varValue = input.section(QChar('='),1).trimmed();

    // check previous output
    if (varValue == PreviousOutput) {
      variableList.insert(varName,previousOutput);
      emit updateVariableList(variableList);
      return previousOutput.toOutput();
    }

    FnData output = compute(varValue);
    previousOutput = output;

    if (output.type() != FailMessage) {
      variableList.insert(varName,output);
      emit updateVariableList(variableList);
    }

    return output.toOutput();

  }

  previousOutput.setFailMessage(tr("Input Error: unknown input %1").arg(input));
  return previousOutput.toOutput();
  
}

/////////////////////////////////////////////////////////
// PRIVATE

FnData Calculator::compute(const QString & input)
{

    FnData output;

    // try to match with integer
    bool isInt;
    int n = input.toInt(&isInt);
    if (isInt) {
        output.setInteger(n);
    }

    // try to match with variable
    else if (variableRegExp.exactMatch(input)) {

        output.setFailMessage(tr("Name Error: \"%1\" is not defined").arg(input));

        // check variable list for input - last _
        output = variableList.value(input.left(input.size() - 1),output);

    }

    // try to match with element
    else if (wordRegExp.exactMatch(input)) {

        output.setFailMessage(tr("Basis Error: %1 is not a word in the basis %2").arg(input).arg(basis));

        FnWord u(input);
        if (u.checkBasis(basis)) {
            u.tighten();
            output.setElement(u);
        }

    }

    // try to match with graph
    else if (graphRegExp.exactMatch(input)) {

        output = loadGraphData(input);

    }

    // try to match with morphism
    else if (morphismRegExp.exactMatch(input)) {

        output = loadMorphismData(input);

      }

    // try to match with list
    else if (listRegExp.exactMatch(input)){

        QString list = input.simplified();
        // remove [ and ]
        list.chop(1);
        list.remove(0,1);

        QStringList listData = breakAtTopLevel(list);
        output = compute(listData.takeFirst());
        foreach(QString listItem, listData) {
            output.addToList(compute(listItem));
        }

    }

    // try to match with preset function
    else if (functionRegExp.exactMatch(input)) {

        // functionName =  everything to the left of first (
        QString functionName = input.section(QChar('('),0,0);
        enum FunctionNames fcn = presetFunctions.fcnTag(functionName);
        QString functionInput = input.section(QChar('('),1);
        functionInput.chop(1);

        FunctionInput converted_input = stringToInput(functionInput);
        output = applyFunction(fcn,converted_input);
    
    }

    else {
        // default returns output
        output.setFailMessage(tr("Input Error: unknown input %1").arg(input));
    }

  return output;

}

FunctionInput Calculator::stringToInput(const QString & input)
{

  QStringList inputList = breakAtTopLevel(input);
  FunctionInput computed_input;

  foreach(QString listItem,inputList) {
    computed_input.append(compute(listItem));
  }

  return computed_input;

}

FnData Calculator::applyFunction(enum FunctionNames fcn, const FunctionInput & input)
{

    FnData failedOutput;

    // a fall through for error messages
    if (input.varTypes().contains(FailMessage)) {
        FunctionInputIterator x(input);
        while (x.hasNext()) {
        failedOutput = x.next();
        if (failedOutput.type() == FailMessage)
            return failedOutput;
        }
    }

    failedOutput.setFailMessage(tr("Function Error: \"%1\" is not a valid function").arg(presetFunctions.fcnName(fcn)));

    switch (fcn) {

    case BicomponentsFcn:
        return BicomponentsFunction(input);
        break;

    case CommutatorFcn:
        return CommutatorFunction(input);
        break;

    case ComposeFcn:
        return ComposeFunction(input);
        break;

    case ConjugateFcn:
        return ConjugationFunction(input);
        break;

    case ConjugacyProblemFcn:
        return ConjugacyProblemFunction(input);
        break;

    case ConnectedComponentsFcn:
        return ConnectedComponentsFunction(input);
        break;

    case ExpFcn:
        return ExponentiationFunction(input);
        break;

    case IdentityFcn:
        return IdentityFunction(input);
        break;

    case InverseFcn:
        return InverseFunction(input);
        break;

    case IsAutomorphismFcn:
        return IsAutomorphismFunction(input);
        break;

    case IsolatedVerticesFcn:
        return IsolatedVerticesFunction(input);
        break;

    case IsPrimitiveElementFcn:
        return IsPrimitiveElementFunction(input);
        break;

    case IsSeparableFcn:
        return IsSeparableFunction(input);
        break;

    case IterateFcn:
        return IterateFunction(input);
        break;

    case LengthFcn:
        return LengthFunction(input);
        break;

    case MapFcn:
        return MapFunction(input);
        break;

    case MultiplyFcn:
        return MultiplyFunction(input);
        break;

    case WhiteheadAutomorphismFcn:
        return WhiteheadAutomorphismFunction(input);
        break;

    case WhiteheadGraphFcn:
        return WhiteheadGraphFunction(input);
        break;

    case WhiteheadProblemFcn:
        return WhiteheadProblemFunction(input);
        break;

    default:
        return failedOutput;
        break;

  }

  return failedOutput;

}

/////////////////////////////////////////////////////////
// PRESET FUNCTIONS

FnData Calculator::BicomponentsFunction(const FunctionInput & input)
{

    FnData output;

    if (!input.isAcceptable(presetFunctions.fcnInput(BicomponentsFcn))) {
      output.setFailMessage(tr("Function Error: invalid input for Bicomponents"));
      return output;
    }

    FnGraph Gamma(input.at(0).graphData());

    QList<FnGraph> bicomponents = Gamma.biconnectedComponents();
    output.setGraphList(bicomponents);

    return output;

}

FnData Calculator::CommutatorFunction(const FunctionInput & input)
{

  FnData output;

  if (!input.isAcceptable(presetFunctions.fcnInput(CommutatorFcn))) {
    output.setFailMessage(tr("Function Error: invalid input for Commutator"));
    return output;
  }

  FnWord u1(input.at(0).wordData());
  FnWord u2(input.at(1).wordData());

  if (!u1.checkBasis(basis)) {
    output.setFailMessage(tr("Basis Error: %1 is not an element in the basis %2").arg(u1).arg(basis));
    return output;
  }

  if (!u2.checkBasis(basis)) {
    output.setFailMessage(tr("Basis Error: %1 is not an element in the basis %2").arg(u2).arg(basis));
    return output;
  }

  FnWord u1u2 = u1*u2*u1.inverse()*u2.inverse();
  output.setElement(u1u2);

  return output;

}

FnData Calculator::ComposeFunction(const FunctionInput &input)
{

  FnData output;

  if (!input.isAcceptable(presetFunctions.fcnInput(ComposeFcn))) {
    output.setFailMessage(tr("Function Error: invalid input for Compose"));
    return output;
  }

  FnMap phi1(input.at(0).mapData());
  FnMap phi2(input.at(1).mapData());

  FnMap phi12 = phi1*phi2;

  if (!phi12) {
    output.setFailMessage(tr("Function Error: image rank (%1) is not equal to domain rank (%2)")
                          .arg(phi2.imageRank()).arg(phi1.domainRank()));
    return output;
  }

  output.setMorphism(phi12);

  return output;

}

FnData Calculator::ConjugationFunction(const FunctionInput & input)
{

  FnData output;

  if (!input.isAcceptable(presetFunctions.fcnInput(ConjugateFcn))) {
    output.setFailMessage(tr("Function Error: invalid input for Conjugate"));
    return output;
  }

  FnWord v(input.at(1).wordData());
  if (!v.checkBasis(basis)) {
    output.setFailMessage(tr("Basis Error: %1 is not an element in the basis %2").arg(v).arg(basis));
    return output;
  }

  if (input.at(0).isList()) {
      QList<FnWord> wordList;

      foreach(FnWord u, input.at(0).wordListData()) {
          if (!u.checkBasis(basis)) {
            output.setFailMessage(tr("Basis Error: %1 is not an element in the basis %2").arg(u).arg(basis));
           return output;
          }
          FnWord u_conj_v = u^v;
          wordList.append(u_conj_v);
      }
      output.setWordList(wordList);
  }
  else {
      FnWord u(input.at(0).wordData());

      if (!u.checkBasis(basis)) {
        output.setFailMessage(tr("Basis Error: %1 is not an element in the basis %2").arg(u).arg(basis));
        return output;
      }

      FnWord u_conj_v = u^v;
      output.setElement(u_conj_v);
  }

  return output;

}

FnData Calculator::ConjugacyProblemFunction(const FunctionInput & input)
{

  FnData output;

  if (!input.isAcceptable(presetFunctions.fcnInput(ConjugacyProblemFcn))) {
    output.setFailMessage(tr("Function Error: invalid input for ConjugacyProblem"));
    return output;
  }

  FnWord u1(input.at(0).wordData());
  FnWord u2(input.at(1).wordData());

  if (!u1.checkBasis(basis)) {
    output.setFailMessage(tr("Basis Error: %1 is not an element in the basis %2").arg(u1).arg(basis));
    return output;
  }

  if (!u2.checkBasis(basis)) {
    output.setFailMessage(tr("Basis Error: %1 is not an element in the basis %2").arg(u2).arg(basis));
    return output;
  }

  FnWord areConjugate = conjugacyProblem(u1,u2);

  if (!areConjugate) {
    output.setFailMessage(tr("%1 and %2 are not conjugate").arg(u1).arg(u2));
    return output;
  }

  output.setElement(areConjugate);

  return output;

}

FnData Calculator::ConnectedComponentsFunction(const FunctionInput & input)
{

    FnData output;

    if (!input.isAcceptable(presetFunctions.fcnInput(ConnectedComponentsFcn))) {
      output.setFailMessage(tr("Function Error: invalid input for ConnectedComponents"));
      return output;
    }

    FnGraph Gamma(input.at(0).graphData());

    QList<FnGraph> components = Gamma.connectedComponents();
    output.setGraphList(components);

    return output;

}

FnData Calculator::ExponentiationFunction(const FunctionInput & input)
{

    FnData output;

    if (!input.isAcceptable(presetFunctions.fcnInput(ExpFcn))) {
        output.setFailMessage(tr("Function Error: invalid input for Exp"));
        return output;
    }

    int n = input.at(1).integerData();

    if (input.at(0).isList()) {
        QList<FnWord> wordList;

        foreach(FnWord u, input.at(0).wordListData()) {
            if (!u.checkBasis(basis)) {
                output.setFailMessage(tr("Basis Error: %1 is not an element in the basis %2").arg(u).arg(basis));
                return output;
            }
            u.tighten();
            FnWord un = u.exp(n);
            wordList.append(un);
        }
        output.setWordList(wordList);
  }
  else {
      FnWord u(input.at(0).wordData());

      if (!u.checkBasis(basis)) {
        output.setFailMessage(tr("Basis Error: %1 is not an element in the basis %2").arg(u).arg(basis));
        return output;
      }
      u.tighten();
      FnWord un = u.exp(n);
      output.setElement(un);
  }

  return output;

}

FnData Calculator::IdentityFunction(const FunctionInput & input)
{

  FnData output;

  if (!input.isAcceptable(presetFunctions.fcnInput(IdentityFcn))) {
    output.setFailMessage(tr("Function Error: invalid input for Identity"));
    return output;
  }

  if (input.at(0).isList()) {
      QList<FnWord> wordList;

      foreach(FnWord u, input.at(0).wordListData()) {
          if (!u.checkBasis(basis)) {
            output.setFailMessage(tr("Basis Error: %1 is not an element in the basis %2").arg(u).arg(basis));
           return output;
          }
          u.tighten();
          wordList.append(u);
      }
      output.setWordList(wordList);
  }
  else {
      FnWord u(input.at(0).wordData());

      if (!u.checkBasis(basis)) {
        output.setFailMessage(tr("Basis Error: %1 is not an element in the basis %2").arg(u).arg(basis));
        return output;
      }
      u.tighten();
      output.setElement(u);
  }

  return output;

}

FnData Calculator::InverseFunction(const FunctionInput & input)
{

  FnData output;

  if (!input.isAcceptable(presetFunctions.fcnInput(InverseFcn))) {
    output.setFailMessage(tr("Function Error: invalid input for Inverse"));
    return output;
  }

  if (input.at(0).isList()) {
      QList<FnWord> wordList;

      foreach(FnWord u, input.at(0).wordListData()) {
          if (!u.checkBasis(basis)) {
            output.setFailMessage(tr("Basis Error: %1 is not an element in the basis %2").arg(u).arg(basis));
           return output;
          }
          u.tighten();
          FnWord U = u.inverse();
          wordList.append(U);
      }
      output.setWordList(wordList);
  }
  else {
      FnWord u(input.at(0).wordData());

      if (!u.checkBasis(basis)) {
        output.setFailMessage(tr("Basis Error: %1 is not an element in the basis %2").arg(u).arg(basis));
        return output;
      }
      u.tighten();
      FnWord U = u.inverse();
      output.setElement(U);
  }

  return output;

}

FnData Calculator::IsAutomorphismFunction(const FunctionInput & input)
{

  FnData output;

  if (!input.isAcceptable(presetFunctions.fcnInput(IsAutomorphismFcn))) {
    output.setFailMessage(tr("Function Error: invalid input for IsAutomorphism"));
    return output;
  }

  FnMap phi(input.at(0).mapData());
  FnMap phi_Inv = phi.isAutomorphism();

  if (!phi_Inv)
    output.setFailMessage(tr("%1 is not an automorphism").arg(input.at(0).toOutput()));
  else
    output.setMorphism(phi_Inv);

  return output;

}

FnData Calculator::IsolatedVerticesFunction(const FunctionInput &input)
{

    FnData output;

    if (!input.isAcceptable(presetFunctions.fcnInput(IsolatedVerticesFcn))) {
      output.setFailMessage(tr("Function Error: invalid input for IsolatedVertices"));
      return output;
    }

    FnGraph Gamma(input.at(0).graphData());

    QList<QString> verticesList = Gamma.isolatedVertices();

    if (verticesList.isEmpty())
        output.setString(tr("There are no isolated vertices"));
    else
        output.setStringList(verticesList);

    return output;

}

FnData Calculator::IsPrimitiveElementFunction(const FunctionInput & input)
{

  FnData output;

  if (!input.isAcceptable(presetFunctions.fcnInput(IsPrimitiveElementFcn))) {
    output.setFailMessage(tr("Function Error: invalid input for IsPrimitiveElement"));
    return output;
  }

  FnWord u(input.at(0).wordData());
  if (input.size() == 2) {
    int r = input.at(1).integerData();
    if (r < Fn_MinRank || r > Fn_MaxRank) {
      output.setFailMessage(tr("Basis Error: %1 is not a valid rank (%2 < rank < %3)").arg(r)
                            .arg(Fn_MinRank).arg(Fn_MaxRank));
      return output;
    }
    basis.changeRank(r);
  }

  if (!u.checkBasis(basis)) {
    output.setFailMessage(tr("Basis Error: %1 is not an element in the basis %2").arg(u).arg(basis));
    return output;
  }

  FnMap phi = isPrimitiveElement(u,basis);

  if (!phi) {
    output.setFailMessage(tr("%1 is not a primitive element").arg(u));
  } else
    output.setMorphism(phi);

  return output;

}

FnData Calculator::IsSeparableFunction(const FunctionInput &input) {

    FnData output;

    if (!input.isAcceptable(presetFunctions.fcnInput(IsSeparableFcn))) {
      output.setFailMessage(tr("Function Error: invalid input for IsSeparable"));
      return output;
    }

    int r = input.at(1).integerData();
    if (r < Fn_MinRank || r > Fn_MaxRank) {
      output.setFailMessage(tr("Basis Error: %1 is not a valid rank (%2 < rank < %3)").arg(r)
                            .arg(Fn_MinRank).arg(Fn_MaxRank));
      return output;
    }
    basis.changeRank(r);

    QList<FnWord> wordList;

    if (input.at(0).isList())
        wordList = input.at(0).wordListData();
    else
        wordList.prepend(input.at(0).wordData());

    foreach(FnWord u, wordList) {
        if (!u.checkBasis(basis)) {
          output.setFailMessage(tr("Basis Error: %1 is not an element in the basis %2").arg(u).arg(basis));
         return output;
        }
    }

    FnWord v = wordList.takeFirst();

    if (v.isSeparable())
        output.setString(tr("%1 is separable.").arg(v));
    else
        output.setString(tr("%1 is not separable.").arg(v));

    return output;

}

FnData Calculator::IterateFunction(const FunctionInput & input)
{

  FnData output;

  if (!input.isAcceptable(presetFunctions.fcnInput(IterateFcn))) {
    output.setFailMessage(tr("Function Error: invalid input for Iterate"));
    return output;
  }

  FnMap phi(input.at(0).mapData());
  int n = input.at(1).integerData();

  if (n < 0) {
    output.setFailMessage(tr("Function Error: number of iterations must be non-negative"));
    return output;
  }

  FnMap phi_n = phi.iterate(n);

  if (!phi_n) {
    output.setFailMessage(tr("Function Error: cannot iterate this morphism"));
    return output;
  }

  output.setMorphism(phi_n);

  return output;

}

FnData Calculator::LengthFunction(const FunctionInput & input)
{

  FnData output;

  if (!input.isAcceptable(presetFunctions.fcnInput(LengthFcn))) {
    output.setFailMessage(tr("Function Error: invalid input for Length"));
    return output;
  }

  if (input.at(0).isList()) {
      QList<int> intList;

      foreach(FnWord u, input.at(0).wordListData()) {
          if (!u.checkBasis(basis)) {
            output.setFailMessage(tr("Basis Error: %1 is not an element in the basis %2").arg(u).arg(basis));
           return output;
          }
          u.tighten();
          if (u == Id)
            intList.append(0);
          else
            intList.append(u.length());
      }
      output.setIntegerList(intList);
  }
  else {
      FnWord u(input.at(0).wordData());

      if (!u.checkBasis(basis)) {
        output.setFailMessage(tr("Basis Error: %1 is not an element in the basis %2").arg(u).arg(basis));
        return output;
      }
      u.tighten();
      if (u == Id)
        output.setInteger(0);
      else
        output.setInteger(u.length());
  }

  return output;

}

FnData Calculator::MapFunction(const FunctionInput & input)
{

    FnData output;

    if (!input.isAcceptable(presetFunctions.fcnInput(MapFcn))) {
        output.setFailMessage(tr("Function Error: invalid input for Map"));
        return output;
    }

    FnMap phi(input.at(0).mapData());
    basis.changeRank(phi.domainRank());

    int n = 1;
    if (input.size() == 3)
        n = input.at(2).integerData();

    if (n < 0) {
        output.setFailMessage(tr("Function Error: number of iterations must be non-negative"));
        return output;
    }

    if(input.at(1).isList()) {
        QList<FnWord> wordList;

        foreach(FnWord u, input.at(1).wordListData()) {
            if (!u.checkBasis(basis)) {
              output.setFailMessage(tr("Basis Error: %1 is not an element in the basis %2").arg(u).arg(basis));
             return output;
            }
            FnWord phi_un = phi(u,n);
            if (!phi_un) {
                output.setFailMessage(tr("Function Error: cannot iterate this morphism"));
                return output;
            }
            wordList.append(phi_un);
        }
        output.setWordList(wordList);

    }
    else {
        FnWord u(input.at(1).wordData());

        if (!u.checkBasis(basis)) {
            output.setFailMessage(tr("Basis Error: %1 is not an element in the basis %2").arg(u).arg(basis));
            return output;
        }
        FnWord phi_un = phi(u,n);
        if (!phi_un) {
            output.setFailMessage(tr("Function Error: cannot iterate this morphism"));
            return output;
        }
        output.setElement(phi_un);
    }

    return output;

}

FnData Calculator::MultiplyFunction(const FunctionInput & input)
{

  FnData output;

  if (!input.isAcceptable(presetFunctions.fcnInput(MultiplyFcn))) {
    output.setFailMessage(tr("Function Error: invalid input for Multiply"));
    return output;
  }

  FnWord u1(input.at(0).wordData());
  FnWord u2(input.at(1).wordData());

  if (!u1.checkBasis(basis)) {
    output.setFailMessage(tr("Basis Error: %1 is not an element in the basis %2").arg(u1).arg(basis));
    return output;
  }

  if (!u2.checkBasis(basis)) {
    output.setFailMessage(tr("Basis Error: %1 is not an element in the basis %2").arg(u2).arg(basis));
    return output;
  }

  FnWord u1u2 = u1*u2;
  output.setElement(u1u2);

  return output;

}

FnData Calculator::WhiteheadAutomorphismFunction(const FunctionInput &input)
{

    FnData output;

    if (!input.isAcceptable(presetFunctions.fcnInput(WhiteheadAutomorphismFcn))) {
      output.setFailMessage(tr("Function Error: invalid input for WhiteheadAutomorphism"));
      return output;
    }

    if (!input.at(0).isList()) {
        output.setFailMessage(tr("Function Error: invalid input for WhiteheadAutomorphism"));
        return output;
    }

    int r = input.at(2).integerData();
    if (r < Fn_MinRank || r > Fn_MaxRank) {
      output.setFailMessage(tr("Basis Error: %1 is not a valid rank (%2 < rank < %3)").arg(r)
                            .arg(Fn_MinRank).arg(Fn_MaxRank));
      return output;
    }
    basis.changeRank(r);

    QString A;
    QChar a = input.at(1).wordData().at(0);
    foreach(FnWord u,input.at(0).wordListData()) {
        if (u.length() != 1) {
            output.setFailMessage(tr("Function Error: %1 must be a list of basis elements").arg(input.at(0).toOutput()));
            return output;
        }
        if (!u.checkBasis(basis)) {
            output.setFailMessage(tr("Basis Error: %1 is not an element in the basis %2").arg(u).arg(basis));
           return output;
        }
        if (A.contains(u)) {
            output.setFailMessage(tr("Function Error: %1 must be a list of basis elements").arg(input.at(0).toOutput()));
            return output;
        }
        A += u;
    }

    if (!A.contains(a)) {
        output.setFailMessage(tr("Function Error: %1 must contain %2").arg(input.at(0).toOutput()).arg(a));
        return output;
    }
    if (A.contains(basis.inverse(a))) {
        output.setFailMessage(tr("Function Error: %1 cannot contain %2").arg(input.at(0).toOutput()).arg(basis.inverse(a)));
        return output;
    }

    WhiteheadData whData(r,A,a);

    FnMap phi = whitehead(whData,basis);

    output.setMorphism(phi);

    return output;

}

FnData Calculator::WhiteheadGraphFunction(const FunctionInput &input)
{

    FnData output;

    if (!input.isAcceptable(presetFunctions.fcnInput(WhiteheadGraphFcn))) {
      output.setFailMessage(tr("Function Error: invalid input for WhiteheadGraph"));
      return output;
    }

    if (input.size() == 2) {
      int r = input.at(1).integerData();
      if (r < Fn_MinRank || r > Fn_MaxRank) {
        output.setFailMessage(tr("Basis Error: %1 is not a valid rank (2 < rank < %2)").arg(r)
                              .arg(Fn_MinRank).arg(Fn_MaxRank));
        return output;
      }
      basis.changeRank(r);
    }

    QList<FnWord> wordList;

    if (input.at(0).isList())
        wordList = input.at(0).wordListData();
    else
        wordList.prepend(input.at(0).wordData());

    foreach(FnWord u, wordList) {
        if (!u.checkBasis(basis)) {
          output.setFailMessage(tr("Basis Error: %1 is not an element in the basis %2").arg(u).arg(basis));
         return output;
        }
    }

    FnGraph whitehead = whiteheadGraph(wordList,basis);
    output.setGraph(whitehead);

    return output;

}

FnData Calculator::WhiteheadProblemFunction(const FunctionInput & input)
{

  FnData output;

  if (!input.isAcceptable(presetFunctions.fcnInput(WhiteheadProblemFcn))) {
    output.setFailMessage(tr("Function Error: invalid input for WhiteheadProblem"));
    return output;
  }

  FnWord u1(input.at(0).wordData());
  FnWord u2(input.at(1).wordData());

  if (input.size() == 3) {
    int r = input.at(2).integerData();
    if (r < Fn_MinRank || r > Fn_MaxRank) {
      output.setFailMessage(tr("Basis Error: %1 is not a valid rank (2 < rank < %2)").arg(r)
                            .arg(Fn_MinRank).arg(Fn_MaxRank));
      return output;
    }
    basis.changeRank(r);
  }

  if (!u1.checkBasis(basis)) {
    output.setFailMessage(tr("Basis Error: %1 is not an element in the basis %2").arg(u1).arg(basis));
    return output;
  }

  if (!u2.checkBasis(basis)) {
    output.setFailMessage(tr("Basis Error: %1 is not an element in the basis %2").arg(u2).arg(basis));
    return output;
  }

  FnMap phi = whiteheadProblem(u1,u2,basis);
  if (!phi)
    output.setFailMessage(tr("%1 and %2 are not related by an automorphism").arg(u1).arg(u2));
  else
    output.setMorphism(phi);

  return output;

}

/////////////////////////////////////////////////////////
// PUBLIC SLOTS

void Calculator::loadGraph(const QString &name, const FnGraph &Gamma)
{

  variableList.insert(name,FnData(Gamma));
  emit updateVariableList(variableList);

}

void Calculator::loadMorphism(const QString &name, const FnMap &phi)
{

  variableList.insert(name,FnData(phi));
  emit updateVariableList(variableList);

}

void Calculator::resetMorphisms()
{

  int r = QMessageBox::warning(0, tr("Free Group Calculator"),
                               tr("Do you want to clear all of the morphisms?\n"
                                  "This cannot be undone."),
                               QMessageBox::Yes
                               | QMessageBox::Cancel);

  if (r == QMessageBox::Yes) {
    VariableListMutableIterator x(variableList);
    while (x.hasNext()) {
      x.next();
      if (x.value().type() == Morphism)
        x.remove();
    }
    emit updateVariableList(variableList);
  }

}

void Calculator::resetGraphs()
{

  int r = QMessageBox::warning(0, tr("Free Group Calculator"),
                               tr("Do you want to clear all of the graphs?\n"
                                  "This cannot be undone."),
                               QMessageBox::Yes
                               | QMessageBox::Cancel);

  if (r == QMessageBox::Yes) {
    VariableListMutableIterator x(variableList);
    while (x.hasNext()) {
      x.next();
      if (x.value().type() == Graph)
        x.remove();
    }
    emit updateVariableList(variableList);
  }

}

void Calculator::resetVariables()
{

    int r = QMessageBox::warning(0, tr("Free Group Calculator"),
                                 tr("Do you want to clear all of the variables?\n"
                                    "This cannot be undone."),
                                 QMessageBox::Yes
                                 | QMessageBox::Cancel);

    if (r == QMessageBox::Yes) {
      VariableListMutableIterator x(variableList);
      while (x.hasNext()) {
        x.next();
        if (x.value().type() == Element || x.value().type() == Integer)
          x.remove();
      }
      variableList.insert(Id,FnData(Id));
      emit updateVariableList(variableList);
    }

}

/////////////////////////////////////////////////////////
// OTHER FUNCTIONS

QStringList breakAtTopLevel(const QString &input)
{

  // returns a list of strings broken at the top level commas

    QString word;
    QString piece;
    QStringList words = input.split(QChar(','));
    QStringList brokenInput;
    int totalLeft = 0;
    int totalRight = 0;
    int leftBracket = 0;
    int rightBracket = 0;
    int leftBrace = 0;
    int rightBrace = 0;

  QStringListIterator i(words);
  while(i.hasNext()) {

    word = i.next();
    piece += word;
    totalLeft += word.count(QChar('('));
    totalRight += word.count(QChar(')'));
    leftBracket += word.count(QChar('{'));
    rightBracket += word.count(QChar('}'));
    leftBrace += word.count(QChar('['));
    rightBrace += word.count(QChar(']'));

    if (totalLeft == totalRight && leftBracket == rightBracket
            && leftBrace == rightBrace) {

      brokenInput.append(piece.trimmed());
      piece.clear();

    } else piece += ",";

  }

  return brokenInput;

}
