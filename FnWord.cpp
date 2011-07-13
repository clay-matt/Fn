//
/////////////////////////////////////////////////////////

#include <QChar>
#include <QRegExp>
#include <QString>

#include "FnWord.h"
                       
/////////////////////////////////////////////////////////
// Private Members

void FnWord::tighten() {

  /* This function tightens the word. */

  int old_length = length();
  int new_length;

  if (*this == Id) return;

  remove(BASIS.tightRegExp);

  new_length = length();

  while (new_length < old_length) {
    old_length = new_length;
    remove(BASIS.tightRegExp);
    new_length = length();
  }

  if (new_length == 0) *this = Id;

  return;

}

FnWord FnWord::cyclicWord() const {

  /* Returns the cyclic word of the element. */

  int i,len;
  FnWord cyc(*this); // initialize cyc

  cyc.tighten(); // tighten first

  if (cyc == Id) return(cyc);

  len = cyc.length(); 
  i = 0;
  while (cyc.at(i) == BASIS.inverse(cyc.at(len-(i+1)))) i++;

  return(cyc.mid(i,len-2*i));

}

FnWord FnWord::inverse() const {

  /* This function returns the inverse of the element. */

  if(*this == Id) return(Id);

  int len;
  FnWord inv;

  len = length();

  while (len > 0) { // write u backwards and swap x <-> X
    len--;
    inv += BASIS.inverse(at(len));
  }

  inv.tighten(); // tighten the inverse

  return(inv);

}

FnWord FnWord::exp(int n) const {

  /* Returns the nth power of the element. */

  FnWord u(*this);

  if (n == 0) return Id;

  else if (n < 0)
    return u.inverse().exp(-n);

  else {
    u *= exp(n-1);
    return u;
  }

}

FnGraph FnWord::whiteheadGraph(const Basis &basis) const {

    FnGraph whitehead;
    QString e="e",num;
    int i=0;
    for(i=0;i<2*basis.getRank();i++)
    {
        whitehead.addVertex(basis.at(i));
    }
    for(i=0;i<length()-1;i++)
    {

        whitehead.addEdge(e+num.setNum(i),at(i),basis.inverse(at(i+1)));
    }
    whitehead.addEdge(e+num.setNum(i), at(i), basis.inverse(at(0)));

    return whitehead;

}

bool FnWord::isSeparable(const Basis &basis) const {

    return true;

}

bool FnWord::operator ! () const {

    return *this == Fail;

}

/////////////////////////////////////////////////////////
// Additional Fuctions

FnWord conjugacyProblem(const FnWord & u, const FnWord & v) {

  /* Tests whether or not u and v are conjugate.  If no, then the
     Fn_word Fail is returned.  If yes, then the element w such that u
     = wvw^-1 is returned. */

  int i,len;
  FnWord u_cyc(u),v_cyc(v);
  FnWord u1,v1;
  
  u_cyc.tighten(); // tighten first
  v_cyc.tighten(); // tighten first

  if (u_cyc == v_cyc) return(Id);

  // find cyclic words and conjugating elements
  i = 0;
  len = u_cyc.length();
  while (u_cyc.at(i) == BASIS.inverse(u_cyc.at(len-(i+1)))) i++;

  u1 = u_cyc.left(i);
  u_cyc = u_cyc.mid(i,len-2*i);
 
  // repeat for v
  i = 0;
  len = v_cyc.length();
  while (v_cyc.at(i) == BASIS.inverse(v_cyc.at(len-(i+1)))) i++;

  v1 = v_cyc.left(i);
  v_cyc = v_cyc.mid(i,len-2*i);

  // now u = u1*u_cyc*(u1)^-1 and v = v1*v_cyc*(v1)^-1

  len = v_cyc.length();
  if (u_cyc.length() != len) return(Fail);
  
  i = 0;
  v_cyc = v_cyc*v_cyc;

  while(i < len) {

    if (v_cyc.mid(i,len) == u_cyc) { // the words are conjugate
      v1 *= v_cyc.left(i);
      return(u1*v1.inverse());
    }
      
    i++;

  }

  return(Fail);

}

FnGraph whiteheadGraph(QList<FnWord> words, const Basis &basis)
{
    FnGraph whitehead;
    for(int i=0;i<words.size();i++)
    {
        whitehead=whitehead+words[i].whiteheadGraph(basis);
    }
    return whitehead;
}

