// Header file for class WhiteheadData
// inherits QString

// author Matt Clay
// version 110224

/////////////////////////////////////////////////////////

#ifndef _WHITEHEADDATA_H // avoid double includes
#define _WHITEHEADDATA_H 

#include <QString>

#include "Fn.h"
#include "FnMap.h"
#include "FnWord.h"

class QChar;

/////////////////////////////////////////////////////////

class WhiteheadData : public QString {

 private:
  int rank;
  QChar a;

 public:
  WhiteheadData() : QString() {
    rank = 0;
  }
  WhiteheadData(int m, const QString & list, const QChar other_a) : QString(list) {
    rank = m;
    a = other_a;
  }

  int getRank() const { return(rank); }
  QChar specialElement() const { return(a); }

};

/////////////////////////////////////////////////////////
// ADDITIONAL FUNCTIONS

FnMap whitehead(const WhiteheadData & A, const Basis & basis);
QList<WhiteheadData> whiteheadAutos(const Basis & basis);

inline int nWhiteheadAutos(const Basis & basis) {
  
  // returns 2r(2**(2r-2) - 1), the number of Whitehead Automorphisms for the given basis

  int r = basis.getRank();
  
  return(2*r*((1 << (2*r-2)) - 1));

}

/////////////////////////////////////////////////////////

#endif // _WHITEHEADDATA_H 
