//
/////////////////////////////////////////////////////////

#include <QChar>
#include <QList>
#include <QRegExp>
#include <QString>
#include "Basis.h"
#include "FnWord.h"
#include "WhiteheadData.h"
                       
/////////////////////////////////////////////////////////
// Private Members

QList<int> FnWord::abelianization(const Basis &basis) const {

  int rank = basis.getRank();
  QList<int> abelian_image;

  for(int i = 0; i < rank; i++) {
    abelian_image.append(count(basis.at(2*i)) - count(basis.at(2*i+1)));
  }

  return abelian_image;

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

FnWord FnWord::exp(int n) const {

  /* Returns the nth power of the element. */

  FnWord u(*this);

  if (n == 0) return Id;

  else if (n < 0)
      return u.inverse().exp(-n);

  else {
      FnWord un(u.repeated(n));
      un.tighten();
      return un;
  }

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

bool FnWord::isSeparable(const Basis &basis) const {
    FnWord word=cyclicWord();
    FnGraph graph=word.whiteheadGraph(basis);
    QList<FnGraph> components=graph.connectedComponents();
    QList<QString> vertices;
    QString setZ;
    QString cutVertex;


    if(components.size()>1)
    {
        return true;
    }
    else  //this part needs to be cleaned up
    {
        //graph.combineMultiEdges();
        components=graph.biconnectedComponents();
        if(components.size()>1)
        {
           vertices=components[0].vertexList();
           for(int i=0;i<vertices.size();i++)
           {
               for(int j=1;j<components.size();j++)
               {
                   if(components[j].vertexList().contains(vertices[i]))
                   {
                       cutVertex=vertices[i];
                       break;
                   }
               }
               if(!cutVertex.isEmpty())
                   break;
           }
           FnGraph temp=graph;
           temp.removeVertex(cutVertex);
           word=cutVertex;
           components=temp.connectedComponents();
           if(components[0].vertexList().contains(word.inverse()))
               temp=graph-components[1];
           else
               temp=graph-components[0];
           temp.removeVertex(word.inverse());
           vertices=temp.vertexList();
           for(int i=0;i<vertices.size();i++)
           {
               setZ+=vertices[i];
           }
           WhiteheadData whData(basis.getRank(),setZ,cutVertex.at(0));
           FnMap phi=whitehead(whData, basis);
           word=phi(*this);
           return word.isSeparable(basis);
        }
    }
    return false;
    
}

QList<int> FnWord::stepTwoNilpotentNormalForm(const Basis & basis) const {
    
    int rank = basis.getRank();
    QList<int> nilpotent_image = abelianization(basis);
    FnWord currentForm(*this);
    FnWord normalForm;

    for(int i = 0; i < rank; i++) {
        FnWord a(basis.at(2*i));
	normalForm *= a.exp(nilpotent_image.at(i));
    }

    QList<int> commutator_portion;

    for(int i = 0; i < (rank*(rank-1))/2; i++) 
	commutator_portion.append(0);

    int move_forward;
    int i,j,commutator;
    int first_index,second_index;
    QChar a_i,a_j;

    while (!currentForm.isTrivial()) {

	// find where the current form and normal form differ
        while(!normalForm.isEmpty()) {
            if(normalForm.at(0) == currentForm.at(0)) {
                currentForm.remove(0,1);
                normalForm.remove(0,1);
            }
            else break;
	}

        if(currentForm.isTrivial()) break;

	if(normalForm.isEmpty()) {
            move_forward = currentForm.indexOf(basis.inverse(currentForm.at(0)));
	} else {
	    move_forward = currentForm.indexOf(normalForm.at(0));
	}
	
	a_i = currentForm.at(move_forward);
	a_j = currentForm.at(move_forward - 1);

	// swap a_ja_i for a_ia_j, so add commutator [a_i,a_j]

	i = basis.indexOf(a_i);
	j = basis.indexOf(a_j);

        first_index = ( i < j ) ? i/2 : j/2;
        second_index = (i > j ) ? i/2 : j/2;
        commutator = first_index*rank - (first_index*(first_index + 1))/2 + (second_index - first_index - 1);

        if((i + j) % 2 == 0) {
            if (i < j)
                commutator_portion[commutator]--;
            else
                commutator_portion[commutator]++;
        } else {
            if (i < j)
                commutator_portion[commutator]++;
            else
                commutator_portion[commutator]--;
        }
	
	currentForm.replace(move_forward - 1,1,a_i);
	currentForm.replace(move_forward,1,a_j);
	currentForm.tighten();
	    
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

bool isSeparable(QList<FnWord> words, const Basis &basis) {

    // determines whether or not a set of words is separable

    QList<FnWord> cycWords;
    foreach(FnWord word, words) { // get cyclic words
        cycWords.append(word.cyclicWord());
    }

    // build Whitehead graph for the words
    FnGraph Gamma = whiteheadGraph(cycWords,basis);

    // connectivity properties don't change by identifying multiedges
    //Gamma.combineMultiEdges();

    // Gamma is disconnected, hence the set of words is separable
    if (Gamma.connectedComponents().size() > 1) return true;

    // get the biconnected components
    QList<FnGraph> bicomponents = Gamma.biconnectedComponents();

    if(bicomponents.size() > 1) { // there is a cut vertex

        QList<QString> V1,V2;
        QString Z;
        FnWord cutVertex;

        FnGraph Gamma1 = bicomponents[0];
        FnGraph Gamma2;

        for( int i = 1; i < bicomponents.size(); i++) {
            Gamma2 = Gamma2 + bicomponents[i];
        }

        V1 = Gamma1.vertexList();
        V2 = Gamma2.vertexList();

        // find cut vertex, it is a vertex appearing in two components
        for(int i = 0; i < V1.size(); i++) {
            for(int j = 1; j < V2.size(); j++) {
                if (V1[i] == V2[j]) {
                    cutVertex = V1[i];
                    break;
                }
            }
        }

        if (V1.contains(cutVertex.inverse())) {
            for(int i=0;i < V1.size(); i++) Z += V1[i];
        } else {
            for(int i=0;i < V2.size(); i++) Z += V2[i];
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


QList<FnWord> wordsoflength (Basis basis, int len){
    QList<FnWord> wordList;

    if(len == 1)
    {
        for(int i = 0;i<basis.size();i++)
        wordList.append(basis.at(i));
        return wordList;
    }

    QList<FnWord> shorterWords = wordsoflength(basis,len-1);
    foreach(FnWord x, shorterWords)
    {

      foreach(QChar a, basis){
        if(basis.inverse(a) != x.at(0))
          wordList.append(a+x);
        }

    }

    return wordList;

}


