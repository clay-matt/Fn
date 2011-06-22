//
/////////////////////////////////////////////////////////

#include <QChar>
#include <QHash>
#include <QList>

#include "Basis.h"
#include "FnMap.h"
#include "FnWord.h"
#include "WhiteheadData.h"

/////////////////////////////////////////////////////////

FnMap whitehead(const WhiteheadData &A, const Basis &basis)
{

  // returns the Whitehead Automorphism for the given data

  QChar x_inv;
  QChar a = A.specialElement();
  QChar a_inv = basis.inverse(a);
  FnWord x_word;
  FnMap phi(basis.getRank());

  MapMutableIterator x(phi);
  while(x.hasNext()) {

    x.next();
    x_word = x.key();
    x_inv = basis.inverse(x.key());
    if (x.key() != a && x.key() != a_inv) {
      if (A.contains(x.key()) && A.contains(x_inv))
        x.setValue((a*x_word)*a_inv);

      if (A.contains(x.key()) && !A.contains(x_inv))
        x.setValue(x_word*a_inv);

      if (!A.contains(x.key()) && A.contains(x_inv))
        x.setValue(a*x_word);
    }

  }

  return phi;
 
}

/////////////////////////////////////////////////////////

QList<WhiteheadData> whiteheadAutos(const Basis &basis)
{

  // creates the data for every possible Whitehead Automorphism for the given basis

  int i,j,k;

  int r = basis.getRank();
  int nAutos = nWhiteheadAutos(basis);
  int m = nAutos/(2*r);

  QChar a;
  QString basis_a;
  QString subset;
  QList<WhiteheadData> whAutos;

  for (i = 0; i < 2*r; i++) {

    a = basis.at(i);

    // build the set without a and A
    basis_a = basis;
    basis_a.remove(a);
    basis_a.remove(basis.inverse(a));

    for (j = 0; j < m; j++) {

      subset = QString(a);
      // build subsets based on the binary expansion of j
      for (k = 0; k < 2*r - 2; k++) {
        if (((j+1) & (1 << k)) != 0)  // true if the kth digit of j+1 is 1
          subset.append(basis_a.at(k));
      }

      whAutos.append(WhiteheadData(r,subset,a));

    } // ends for loop on j

  } // ends for loop on i

  return whAutos;

}

/////////////////////////////////////////////////////////
