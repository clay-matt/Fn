//
/////////////////////////////////////////////////////////

#include <QChar>
#include <QList>
#include <QString>

#include "FnMap.h"
#include "WhiteheadData.h"
                       
/////////////////////////////////////////////////////////
// CONSTRUCTORS

FnMap::FnMap(int rank) : QHash<QChar, FnWord>()
{

  // constructs the identity function
   image_rank = rank;

   if (rank < Fn_MinRank || rank > Fn_MaxRank) { // check MIN_RANK <= rank <= MAX_RANK
     insert(BASIS.at(0),Fail);
     return;
   }

   for(int i = 0; i < 2*rank; i++) {
     insert(BASIS.at(i),FnWord(BASIS.at(i)));
   }

 }

FnMap::FnMap(const QStringList & images, int rank) : QHash<QChar, FnWord>()
{

  // rank = 0 means build an endomorphism
  image_rank = (rank == 0) ? images.size() : rank;

  if (image_rank < Fn_MinRank || image_rank > Fn_MaxRank) { // check MIN_RANK <= rank <= MAX_RANK
    insert(BASIS.at(0),Fail);
    return;
  }

  if (images.size() < Fn_MinRank || images.size() > Fn_MaxRank) {
    insert(BASIS.at(0),Fail);
    return;
  }

  Basis basis(image_rank);
  for (int i = 0; i < images.size(); i++) {

    FnWord u(images.at(i));
    if (!u.checkBasis(basis)) {
      insert(BASIS.at(0),Fail);
      return;
    }

    insert(BASIS.at(2*i),u);
    insert(BASIS.at(2*i+1),u.inverse());

  }

}

/////////////////////////////////////////////////////////
// PUBLIC MEMBER FUNCTIONS

FnMap FnMap::isAutomorphism() const
{

  /* Test to see is map defines an automorphism.  Can be used to check
     if a given collection words defines a basis.  If the map is not
     an automorphism, the Fail map is returned. */

  FnMap phi(image_rank);

  // check that map is an endomorphism
  if (size() != 2*image_rank) {
    phi.fail();
    return phi;
  }

  int old_norm,new_norm;
  FnMap tmp(image_rank);
  FnMap this_min(*this);
  FnMap permutation(image_rank);
  FnMap whAuto(image_rank);
  QChar a;

  Basis basis(size()/2);
  QList<WhiteheadData> whAutos = whiteheadAutos(basis);
  QListIterator<WhiteheadData> move(whAutos);

  bool reduced_norm = true;

  while (reduced_norm) {
    
    old_norm = this_min.norm();
    new_norm = old_norm;
    move.toFront();

    while (old_norm <= new_norm && move.hasNext()) {
      whAuto = whitehead(move.next(),basis);
      tmp = whAuto*this_min;
      new_norm = tmp.norm();
    } 

    if (new_norm < old_norm) {
      this_min = tmp;
      phi = whAuto*phi; // build inverse
    } else
      reduced_norm = false;

  } // end while(reduced_norm)

  // now this_min = phi*this

  QString basisElements = basis;
  MapIterator x(this_min);
  while (x.hasNext()) {
    x.next();
    if (x.value() == Id || x.value().length() != 1) {
      phi.fail();
      return phi;
    }
    a = x.value().at(0);
    basisElements.remove(a);
    permutation.insert(a,FnWord(x.key()));
  }

  if (!basisElements.isEmpty()) {
    phi.fail();
    return phi;
  }

  return permutation*phi;

}

FnMap FnMap::iterate(int n) const
{

    //returns the nth power of the automorphism
    //uses repeated squaring

    int i;
    FnMap phi_pow2(*this);
    FnMap phi_n(image_rank);

    // special cases
    if (n == 1) {
        return FnMap(*this);
    }
    if (phi_pow2.imageRank() != phi_pow2.domainRank()) {
      phi_n.fail();
      return phi_n;
    }
    if (n < 0)  { // eventually want this to take you to the inverse if possible...
      phi_n.fail();
      return phi_n;
    }

    QString n_base2 = QString::number(n,2); // convert n to base 2

    for (i = n_base2.length() - 1; i > 0; i--) {
         // if the ith digit in the base 2 representation of n is 1,
         // multiply phi_n by this power
         if(n_base2.at(i).digitValue() == 1) phi_n *= phi_pow2;
         // now square
         phi_pow2 *= phi_pow2;
    }

    // the first digit is always nonzero
    phi_n *= phi_pow2;

    return phi_n;

}

/////////////////////////////////////////////////////////
// OPERATORS

FnWord FnMap::operator () (const FnWord &u, int niter) const
{

    // returns the image of u upon applying the map niter times

    // special cases
    if (niter == 0) return u;
    if (niter < 0) return Fail;
    if (u == Id) return Id;
    if (niter > 1 && image_rank != domainRank()) return Fail;

    int i;
    FnMap phi_n;
    FnWord image;

    phi_n = iterate(niter);

    for (i = 0; i < u.length(); i++) {
      image += phi_n.value(u.at(i));
    }

    image.tighten();

    return image;
}

FnMap & FnMap::operator *= (const FnMap &phi) {

    // composes phi * this

    FnMap this_map(*this); // make a copy of this morphism
    FnMap phi_copy(phi); // make a copy of phi incase phi = this

    // check that morphisms can be composed
    if (image_rank != phi_copy.domainRank()) {
        fail();
        return *this;
    }

    // set new image rank
    image_rank = phi_copy.imageRank();

    int i;
    FnWord this_image,phi_image;
    MapIterator x(this_map);
    while (x.hasNext()) {
        x.next();
        this_image = x.value();
        if (this_image == Id) {
            phi_image = Id;
        } else {
            phi_image = FnWord(QString("")); // reset phi_image
            for (i = 0; i < this_image.length(); i++) {
                phi_image += phi_copy.value(this_image.at(i));
            }
            phi_image.tighten();
        }
        insert(x.key(),phi_image);
    }

    return *this;

}

//FRIEND OPERATORS

FnMap operator * (const FnMap &phi, const FnMap &psi)
{

    // composes the maps phi and psi
    FnMap phi_psi(psi);
    phi_psi *= phi;
    return phi_psi;

}

/////////////////////////////////////////////////////////
// ADDITIONAL FUNCTIONS

FnMap minimizeLength(const FnWord &u, const Basis &basis)
{

  /* Returns an automorphism phi such that phi(u) has minimal
     length. */

  int r = basis.getRank();
  FnMap phi(r);

  if (u == Id) return phi;
  if (u.length() == 1) return phi;

  int old_norm,new_norm;
  FnWord u_min(u),tmp;
  FnMap whAuto(r);

  QList<WhiteheadData> whAutos = whiteheadAutos(basis);
  QListIterator<WhiteheadData> move(whAutos);

  bool reduced_norm = true;

  while (reduced_norm) {

    old_norm = u_min.length();
    new_norm = old_norm;
    move.toFront();

    while (old_norm <= new_norm && move.hasNext()) {
      whAuto = whitehead(move.next(),basis);
      tmp = whAuto(u_min);
      new_norm = tmp.length();
    }
    if (new_norm < old_norm) {
      u_min = tmp;
      phi = whAuto*phi;
    } else
      reduced_norm = false;
  } // end while (reduced_norm)
 
  // now u_min = phi(u)

  return phi;

} 

FnMap isPrimitiveElement(const FnWord &u, const Basis &basis)
{

  /* Determines if u is a primitive element.  If it is, an automorphism
     phi such that phi(u) has length 1 is returned.  If not, the map
     returned is the Fail map. */

  FnMap phi;

  if (u == Id) {
    phi.fail();
    return phi;
  }

  phi = minimizeLength(u,basis);
  FnWord u_min = phi(u);

  if (u_min.length() != 1) { // u is not a basis element
    phi.fail();
  } 

  //else u is a primitve element

  return phi;

}

FnMap whiteheadProblem(const FnWord &u, const FnWord &v,
                         const Basis &basis)
{

  /* Determines if there is an automorphism phi such that u = phi(v).
     If yes, the map is returned.  Else, the Fail map is returned. */

  int i,r = basis.getRank();
  int min_len;
  FnWord tmp,v_tmp;
  FnWord u_min,v_min;
  FnMap phi(r);
  FnMap whAuto(r);
  FnMap rho(r),sigma(r),tau(r);
  FnMap phi_u(r),phi_v(r),phi_u_inv(r);

  // get minimal length representatives
  phi_u = minimizeLength(u,basis);
  phi_v = minimizeLength(v,basis);

  u_min = phi_u(u);
  v_min = phi_v(v);

  // easy case, minimum lengths are different
  if (u_min.length() != v_min.length()) { 
    phi.fail();
    return phi;
  }

  min_len = u_min.length();
  phi_u_inv = phi_u.isAutomorphism();

  // other easy case, u_min = v_min
  if (u_min == v_min) { // u = phi_u^-1 (phi_v(v))
    phi = phi_u_inv*phi_v;
    return phi;
  }

  // now the hard case... need to find all words of length u_min related
  // by Whitehead moves and permutations...

  // build Whitehead data
  QList<WhiteheadData> whAutos = whiteheadAutos(basis);
  QListIterator<WhiteheadData> move(whAutos);

  // define the permutation automorphisms
  for (i = 0; i < r-1; i++) { // i -> i+1
    rho.insert(basis.at(2*i),FnWord(basis.at(2*i + 2)));
    rho.insert(basis.at(2*i + 1),FnWord(basis.at(2*i + 3)));
  }
  rho.insert(basis.at(2*r - 2),FnWord(basis.at(0)));
  rho.insert(basis.at(2*r - 1),FnWord(basis.at(1)));

  sigma.insert(basis.at(0),FnWord(basis.at(1))); // a -> A
  sigma.insert(basis.at(1),FnWord(basis.at(0))); // A -> a

  tau.insert(basis.at(0),FnWord(basis.at(2))); // a -> b
  tau.insert(basis.at(1),FnWord(basis.at(3))); // A -> B
  tau.insert(basis.at(2),FnWord(basis.at(0))); // b -> a
  tau.insert(basis.at(3),FnWord(basis.at(1))); // B -> A

  int n = 0;
  QList<FnWord> v_rel;
  QHash<FnWord, FnMap> wh_v; // wh_v[x](v_min) = x

  v_rel.append(v_min);
  wh_v.insert(v_min,FnMap(r));

  // looped over words with the same length as v
  while (n < v_rel.size()) {

    v_tmp = v_rel.at(n++);
    move.toFront();

    // loop over Whitehead moves
    while (move.hasNext()) {
      whAuto = whitehead(move.next(),basis);
      tmp = whAuto(v_tmp);
      if (tmp.length() == min_len) {
        if (!v_rel.contains(tmp)) {
          v_rel.append(tmp);
          wh_v.insert(tmp,whAuto*wh_v.value(v_tmp));
        }
        if (tmp == u_min) { // found it!
          phi_v = wh_v.value(tmp)*phi_v; // phi_u(u) = tmp = phi_v(v)
          phi = phi_u_inv*phi_v; // u = phi_u^-1 phi_v(v)
          return phi;
        }
      }
    } // end while (move.hasNext())

    // loop over permutation moves, these don't change length
    tmp = rho(v_tmp);
    if (!v_rel.contains(tmp)) {
      v_rel.append(tmp);
      wh_v.insert(tmp,rho*wh_v.value(v_tmp));
    }
    if (tmp == u_min) { // found it!
      phi_v = wh_v.value(tmp)*phi_v; // phi_u(u) = tmp = phi_v(v)
      phi = phi_u_inv*phi_v; // u = phi_u^-1 phi_v(v)
      return phi;
    }

    tmp = sigma(v_tmp);
    if (!v_rel.contains(tmp)) {
      v_rel.append(tmp);
      wh_v.insert(tmp,sigma*wh_v.value(v_tmp));
    }
    if (tmp == u_min) { // found it!
      phi_v = wh_v.value(tmp)*phi_v; // phi_u(u) = tmp = phi_v(v)
      phi = phi_u_inv*phi_v; // u = phi_u^-1 phi_v(v)
      return phi;
    }

    tmp = tau(v_tmp);
    if (!v_rel.contains(tmp)) {
      v_rel.append(tmp);
      wh_v.insert(tmp,tau*wh_v.value(v_tmp));
    }
    if (tmp == u_min) { // found it!
      phi_v = wh_v.value(tmp)*phi_v; // phi_u(u) = tmp = phi_v(v)
      phi = phi_u_inv*phi_v; // u = phi_u^-1 phi_v(v)
      return phi;
    }

  } // end while (x.hasNext())

  // didn't find it
  phi.fail();

  return phi;

}

/////////////////////////////////////////////////////////

