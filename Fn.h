// Header file for constants

// author Matt Clay
// version 110713

/////////////////////////////////////////////////////////

#ifndef FN_H // avoid double includes
#define FN_H

#include <QDateTime>
#include <QString>

enum VariableTypes {NoType,
                    Element,
                    Graph,
                    Integer,
                    Morphism,
                    OptionalInteger,
                    String,
                    Listable,
                    NotListable,
                    FailMessage};

enum FunctionNames {ERROR,
                    BicomponentsFcn,
                    CommutatorFcn,
                    ComposeFcn,
                    ConjugateFcn,
                    ConjugacyProblemFcn,
                    ConnectedComponentsFcn,
                    ExpFcn,
                    IdentityFcn,
                    InverseFcn,
                    IsAutomorphismFcn,
                    IsolatedVerticesFcn,
                    IsPrimitiveElementFcn,
                    IsSeparableFcn,
                    IterateFcn,
                    LengthFcn,
                    MapFcn,
                    MultiplyFcn,
                    WhiteheadAutomorphismFcn,
                    WhiteheadGraphFcn,
                    WhiteheadProblemFcn};

const int Fn_MinRank = 2;
const int Fn_MaxRank = 10;
const int Fn_DefaultRank = 2;
const int Fn_MaxVertices = 100;
const int Fn_MaxEdges = 100;
const char Fn_BasisCharacters[] = "aAbBcCdDeEfFgGhHiIjJ";
const char Fn_TightRegExp[] = "aA|Aa|bB|Bb|cC|Cc|dD|Dd|eE|Ee|fF|Ff|gG|Gg|hH|Hh|iI|Ii|jJ|Jj";
const char Fn_Identity[] = "1";

// file handling constants
const QString FnVersion = "0.1";
const QString updateDate = QDate::currentDate().toString();

#endif // FN_H
