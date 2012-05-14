//
/////////////////////////////////////////////////////////

#include <QChar>
#include <QList>
#include <QRegExp>
#include <QString>

#include <QDebug>

#include "Basis.h"
#include "FnWord.h"
#include "WhiteheadData.h"
                       
/////////////////////////////////////////////////////////
// Private Members

QList<int> FnWord::abelianization(const Basis &basis) const {

    // returns an integer list of basis elements exponent sums

    int rank = basis.getRank();
    QList<int> abelian_image;

    for(int i = 0; i < rank; i++)
        abelian_image.append(count(basis.at(2*i)) - count(basis.at(2*i+1)));

    return abelian_image;

}

FnWord FnWord::cyclicWord() const {

    // returns the cyclic word of the element

    int i,len;
    FnWord cyc(*this);

    cyc.tighten(); // tighten first

    if (cyc == Id) return(cyc);

    len = cyc.length();
    i = 0;

    while (cyc.at(i) == BASIS.inverse(cyc.at(len-(i+1)))) i++;

    return(cyc.mid(i,len-2*i));

}

FnWord FnWord::exp(int n) const {

    // returns the nth power of the element

    // if n = 0, then the identity is returned

    if (n == 0) return Id;

    else if (n < 0)
        return inverse().exp(-n);

    else {
        FnWord un(repeated(n));
        un.tighten();
        return un;
    }

}

FnWord FnWord::inverse() const {

    // returns the inverse of the element

    if(*this == Id) return(Id);

    FnWord inv;

    int len = length();

    while (len > 0) { // write u backwards and swap x <-> X
        len--;
        inv += BASIS.inverse(at(len));
    }

    inv.tighten();

    return(inv);

}

bool FnWord::isSeparableElement(const Basis &basis) const {

    // returns true if the element is separable by using Stallings algorithm

    QList<FnWord> word;

    word.append(*this);

    return isSeparable(word,basis);

}

QList<int> FnWord::stepTwoNilpotentNormalForm(const Basis & basis) const {
    
    // returns an integer list of the two step nilpotent normal form of the element

    int rank = basis.getRank();
    QList<int> nilpotent_image = abelianization(basis);
    QList<int> commutator_portion;

    int index = 0;
    for(int i = 0; i < rank; i++) {
        for(int j = i+1; j < rank; j++) {

            QString ijBasis(basis);
            ijBasis.remove(2*j,2);
            ijBasis.remove(2*i,2);

            FnWord ijWord(*this);
            ijWord.remove(QRegExp("["+ijBasis+"]"));
            ijWord.tighten();

            // integrate -y dx
            int integral = 0;
            commutator_portion.append(0);
            for(int k = 0; k < ijWord.length(); k++) {

                if(ijWord.at(k) == basis.at(2*i))
                    commutator_portion[index] += integral;
                if(ijWord.at(k) == basis.at(2*i + 1))
                    commutator_portion[index] -= integral;
                if(ijWord.at(k) == basis.at(2*j))
                    integral -= 1;
                if(ijWord.at(k) == basis.at(2*j + 1))
                    integral += 1;

            }

            index++;

        }

    }

    nilpotent_image.append(commutator_portion);

    return nilpotent_image;

}

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

FnGraph FnWord::whiteheadGraph(const Basis &basis) const {

    FnGraph whitehead;
    QString edge;
    int i;

    for(i = 0; i < 2*basis.getRank(); i++)
        whitehead.addVertex(basis.at(i));

    for(i = 0; i < length()-1; i++)
        whitehead.addEdge(edge.setNum(i),at(i),basis.inverse(at(i+1)));

    whitehead.addEdge(edge.setNum(i),at(i),basis.inverse(at(0)));

    return whitehead;

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

bool isSeparable(const QList<FnWord> words, const Basis &basis) {

    // determines whether or not a set of words is separable

    QList<FnWord> cycWords;
    foreach(FnWord word, words) { // get cyclic words
        cycWords.append(word.cyclicWord());
    }

    // build Whitehead graph for the words
    FnGraph Gamma = whiteheadGraph(cycWords,basis);

    // Gamma is disconnected, hence the set of words is separable
    if (Gamma.connectedComponents().size() > 1) return true;

    // get the biconnected components
    QList<FnGraph> bicomponents = Gamma.biconnectedComponents();

    if(bicomponents.size() > 1) { // there is a cut vertex

        QStringList V1,V2; // vertex sets
        QString Z;  // vertex set as string for use by WhiteheadData
        FnWord cutVertex;

        FnGraph Gamma1 = bicomponents.at(0);
        FnGraph Gamma2 = bicomponents.at(1);

        // add remaining components
        for( int i = 2; i < bicomponents.size(); i++) {
            Gamma2 = Gamma2 + bicomponents.at(i);
        }

        V1 = Gamma1.vertexList();
        V2 = Gamma2.vertexList();

        // find cut vertex, it is the vertex that appears in both V1 and V2
        for(int i = 0; i < V1.size(); i++) {
            for(int j = 1; j < V2.size(); j++) {
                if (V1.at(i) == V2.at(j)) {
                    cutVertex = V1.at(i);
                    break;
                }
            }
        }

        // build appropriate Whitehead auto
        if (V1.contains(cutVertex.inverse())) {
            Z = V1.join("");
        } else {
            Z = V2.join("");
        }

        WhiteheadData whData(basis.getRank(),Z,cutVertex.at(0));
        FnMap phi = whitehead(whData, basis);

        // shorten the words according
        QList<FnWord> newWords;
        foreach(FnWord word,cycWords) {
            newWords.append(phi(word));
        }

        // rerun algorithm again
        return isSeparable(newWords,basis);

    }

    // there is no cut vertex, so the set of words is not separable
    return false;

}

FnGraph whiteheadGraph(QList<FnWord> words, const Basis &basis)
{

    // returns the Whitehead graph of a collection of words

    int i,nword;
    QString edgeNumber,wordNumber;
    FnGraph whitehead;

    // add vertices to graph
    for(i = 0; i < 2*basis.getRank(); i++)
        whitehead.addVertex(basis.at(i));

    nword = 1;
    foreach(FnWord word, words) {

        wordNumber.setNum(nword);

        // add edges for word
        for(i = 0; i < word.length()-1; i++)
            whitehead.addEdge(wordNumber+"."+edgeNumber.setNum(i),word.at(i),basis.inverse(word.at(i+1)));

        whitehead.addEdge(wordNumber+"."+edgeNumber.setNum(i),word.at(i),basis.inverse(word.at(0)));

        nword++;

    }

    return whitehead;

}


QList<FnWord> wordsOfLength (const Basis &basis, int len){
    QList<FnWord> wordList;

    if(len == 1)
    {
        for(int i = 0;i<basis.size();i++)
        wordList.append(basis.at(i));
        return wordList;
    }

    QList<FnWord> shorterWords = wordsOfLength(basis,len-1);
    foreach(FnWord x, shorterWords)
    {

      foreach(QChar a, basis){
        if(basis.inverse(a) != x.at(0))
          wordList.append(a+x);
        }

    }

    return wordList;

}


