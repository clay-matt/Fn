// Header file for class FnMap
// inherits QHash<QChar, FnWord>

// author Matt Clay
// version 110525

/////////////////////////////////////////////////////////

#ifndef FNMAP_H // avoid double includes
#define FNMAP_H

#include <QChar>
#include <QHash>
#include <QString>
#include <QStringList>

#include "Basis.h"
#include "Fn.h"
#include "FnWord.h"

/////////////////////////////////////////////////////////

class FnMap : public QHash<QChar, FnWord> {
    
  /* When programming with this class, reserve the
     variables phi and psi to denote typical automorphisms. */

    private:
    int image_rank;

    public:
    FnMap(int rank = Fn_MaxRank);
    FnMap(const QStringList &images, int rank = 0); // rank = 0 is endomorphism

    // member functions
    int domainRank(void) const { return(size()/2); }
    int imageRank(void) const { return(image_rank); }
    int norm(void) const {
        int auto_norm = 0;
        foreach(FnWord u, *this)
            if (u != Id) auto_norm += u.length();
        return(auto_norm/2);
    }
    void fail() { insert(BASIS.at(0),Fail); }
    bool operator ! () const { return (!value(BASIS.at(0))); } // true means it failed
    FnMap isAutomorphism(void) const;
    FnMap iterate(int n) const;
    FnWord operator () (const FnWord &u, int niter = 1) const;
    FnMap & operator *= (const FnMap &phi);

    // friends
    friend FnMap operator * (const FnMap &phi, const FnMap &psi);
  
};

typedef QHashIterator<QChar, FnWord> MapIterator;
typedef QMutableHashIterator<QChar, FnWord> MapMutableIterator;

/////////////////////////////////////////////////////////

// additional operators

inline bool operator == (const FnMap &phi, const FnMap &psi) {

  if (phi.imageRank() != psi.imageRank()) return(false);
  
  return (QHash<QChar, FnWord>)phi == (QHash<QChar, FnWord>)psi;
}

inline int operator != (const FnMap &phi, const FnMap &psi) {

  return(!(phi == psi));
  
}

// additional functions

FnMap minimizeLength(const FnWord &u, const Basis &basis);
FnMap isPrimitiveElement(const FnWord &u, const Basis &basis);
FnMap whiteheadProblem(const FnWord &u, const FnWord &v, const Basis &basis);

/////////////////////////////////////////////////////////

#endif // FNMAP_H
