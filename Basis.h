// Header file for class Basis
// inherits QString

// author Matt Clay
// version 110525

/////////////////////////////////////////////////////////

#ifndef BASIS_H // avoid double includes
#define BASIS_H

#include <QRegExp>
#include <QString>

#include "Fn.h"

class FnWord;

class Basis : public QString {

private:
    int rank;
    QRegExp basisRegExp;
    QRegExp tightRegExp;

public:
    Basis(int r = Fn_DefaultRank);
    Basis(QString basisCharacters);

    QRegExp basisValidate(void) const;
    void changeRank(int r);
    int getRank(void) const;
    bool inBasis(const QString & u) const;
    QChar inverse(QChar x) const;

    friend class FnWord;

};

const Basis BASIS(QString("1aAbBcCdDeEfFgGhHiIjJ"));

#endif // BASIS_H
