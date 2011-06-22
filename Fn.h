// Header file for constants

// author Matt Clay
// version 110524

/////////////////////////////////////////////////////////

#ifndef FN_H // avoid double includes
#define FN_H

#include <QDateTime>
#include <QString>

enum VariableTypes {NoType,
                    Element,
                    FailMessage,
                    Graph,
                    Integer,
                    Morphism,
                    OptionalInteger};

enum FunctionNames {ERROR,
                    CommutatorFcn,
                    ComposeFcn,
                    ConjugateFcn,
                    ConjugacyProblemFcn,
                    ExpFcn,
                    IdentityFcn,
                    InverseFcn,
                    IsAutomorphismFcn,
                    IsPrimitiveElementFcn,
                    IterateFcn,
                    LengthFcn,
                    MapFcn,
                    MultiplyFcn,
                    WhiteheadProblemFcn};

const int Fn_MinRank = 2;
const int Fn_MaxRank = 10;
const int Fn_DefaultRank = 2;
const int Fn_MaxVertices = 100;
const int Fn_MaxEdges = 100;
const QString Fn_BasisCharacters = "aAbBcCdDeEfFgGhHiIjJ";
const QString Fn_TightRegExp = "aA|Aa|bB|Bb|cC|Cc|dD|Dd|eE|Ee|fF|Ff|gG|Gg|hH|Hh|iI|Ii|jJ|Jj";
const QString Fn_Identity = "1";

// file handling constants
const QString FnVersion = "0.1";
const QString updateDate = QDate::currentDate().toString();

#endif // FN_H
