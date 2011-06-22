//
/////////////////////////////////////////////////////////

#include "Basis.h"

/////////////////////////////////////////////////////////

Basis::Basis(int r) : QString(Fn_BasisCharacters)
{

  // make sure MinRank <= r <= MaxRank, if not set to DefaultRank
  if(r < Fn_MinRank || r > Fn_MaxRank) r = Fn_DefaultRank;
  resize(2*r);
  rank = r;
  basisRegExp.setPattern(QString(Fn_Identity) + QString("|[" + *this + "]+"));
  tightRegExp.setPattern(QString(Fn_Identity) + "|" + QString(Fn_TightRegExp).left(6*rank-1));

}

/////////////////////////////////////////////////////////
// PUBLIC

QRegExp Basis::basisValidate() const
{

  return(basisRegExp);

}

void Basis::changeRank(int r)
{

  if (rank != r) {
    Basis new_basis(r); // automatically checks that r is in the correct range
    *this = new_basis;
  }

}

int Basis::getRank() const
{

  return(rank);

}

bool Basis::inBasis(const QString &u) const
{

  return basisRegExp.exactMatch(u);

}

QChar Basis::inverse(QChar x) const
{

  int index = indexOf(x);
  if (index == -1) return(QChar('0')); // not in basis
  if (index % 2 == 0) return(at(index+1)); // lowercase
  else return(at(index-1)); // uppercase

}
