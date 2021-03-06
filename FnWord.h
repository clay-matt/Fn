// Header file for class FnWord
// inherits QString

// authors Matt Clay, Jack Conant and Nivetha Ramasubramanian
// version 120514

/////////////////////////////////////////////////////////

#ifndef FNWORD_H // avoid double includes
#define FNWORD_H

#include <QChar>
#include <QString>

#include "Basis.h"
#include "Fn.h"
#include "FnGraph.h"

class FnWord : public QString {
    
     /* When programming with this class, reserve the
        letters u and v to denote typical elements of Fn. */

    public:
    FnWord() : QString() {}
    FnWord(const QString &other) : QString(other) {}
    FnWord(const QChar a) : QString(a) {}

    // member functions
    FnWord & operator = (const QChar a) {
        *this = QString(a);
        return(*this);
    }
    FnWord & operator *= (const FnWord &u) {
        if (u == Fn_Identity) return(*this);
        if (*this == Fn_Identity) {
            *this = u;
            return(*this);
        }
        *this += u;
        tighten();
        return(*this);
    }
    FnWord & operator *= (const QChar a) {
        if (QString(a) == Fn_Identity) return(*this);
        if (*this == Fn_Identity) {
            *this = a;
            return(*this);
        }
        *this += a;
        tighten();
        return(*this);
    }
    bool checkBasis (const Basis &basis = BASIS) const {
        return basis.inBasis(*this);
    }
    bool isTrivial () const {
        if (*this == Fn_Identity) return true;
        return isEmpty();
    }
    QList<int> abelianization(const Basis &basis = BASIS) const;
    FnWord cyclicWord() const;
    FnWord exp(int n) const;
    FnWord inverse() const;
    bool isSeparableElement(const Basis &basis = BASIS) const;
    QList<int> stepTwoNilpotentNormalForm(const Basis &basis = BASIS) const;
    void tighten();
    FnGraph whiteheadGraph(const Basis &basis = BASIS) const;
    bool operator ! () const;
    
    // friends
    friend FnWord operator * (const FnWord &u, const FnWord &v);
    friend FnWord operator * (const QChar a, const FnWord &v);
    friend FnWord operator * (const FnWord &u, const QChar a);
    friend FnWord operator ^ (const FnWord &u, const FnWord &v);
    friend FnWord operator ^ (const FnWord &u, const QChar a);
    friend FnWord conjugacyProblem(const FnWord &u, const FnWord &v);
    friend bool isSeparable(const QList<FnWord> words, const Basis &basis);
    friend FnGraph whiteheadGraph(QList<FnWord> words, const Basis &basis);

};

const FnWord Id(Fn_Identity);
const FnWord Fail(QString("Fail 0"));

FnWord conjugacyProblem(const FnWord &u, const FnWord &v);
bool isSeparable(QList<FnWord> words, const Basis &basis);
FnGraph whiteheadGraph(QList<FnWord> words, const Basis &basis = BASIS);
QList<FnWord> wordsOfLength(const Basis &basis, int len);

/////////////////////////////////////////////////////////

// additional operators
inline FnWord operator * (const FnWord & u, const FnWord & v) {

    FnWord uv(u);
    uv *= v;
    return(uv);
  
}

inline FnWord operator * (const QChar a, const FnWord & v) {

    FnWord av(a);
    av *= v;
    return(av);

}

inline FnWord operator * (const FnWord & u, const QChar a) {

    FnWord ua(u);
    ua *= a;
    return(ua);

}

inline FnWord operator ^ (const FnWord & u, const FnWord & v) {
  
  return((v*u)*v.inverse());

}

inline FnWord operator ^ (const FnWord & u, const QChar a) {

    return(u^FnWord(a));

}

#endif // FNWORD_H_
