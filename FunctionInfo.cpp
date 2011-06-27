//

#include "Fn.h"
#include "FunctionInfo.h"

FunctionInfo::FunctionInfo(QObject *parent) :
    QObject(parent)
{

  // place function tags into list
  functionEnum["Commutator_"] = CommutatorFcn;
  functionEnum["Compose_"] = ComposeFcn;
  functionEnum["Conjugate_"] = ConjugateFcn;
  functionEnum["ConjugacyProblem_"] = ConjugacyProblemFcn;
  functionEnum["ConnectedComponents_"] = ConnectedComponentsFcn;
  functionEnum["Exp_"] = ExpFcn;
  functionEnum["Identity_"] = IdentityFcn;
  functionEnum["Inverse_"] = InverseFcn;
  functionEnum["IsAutomorphism_"] = IsAutomorphismFcn;
  functionEnum["IsolatedVertices_"] = IsolatedVerticesFcn;
  functionEnum["IsPrimitiveElement_"] = IsPrimitiveElementFcn;
  functionEnum["Iterate_"] = IterateFcn;
  functionEnum["Length_"] = LengthFcn;
  functionEnum["Map_"] = MapFcn;
  functionEnum["Multiply_"] = MultiplyFcn;
  functionEnum["WhiteheadGraph_"] = WhiteheadGraphFcn;
  functionEnum["WhiteheadProblem_"] = WhiteheadProblemFcn;

  // place function names into list
  functionName[CommutatorFcn] = "Commutator_";
  functionName[ComposeFcn] = "Compose_";
  functionName[ConjugateFcn] = "Conjugate_";
  functionName[ConjugacyProblemFcn] = "ConjugacyProblem_";
  functionName[ConnectedComponentsFcn] = "ConnectedComponents_";
  functionName[ExpFcn] = "Exp_";
  functionName[IdentityFcn] = "Identity_";
  functionName[InverseFcn] = "Inverse_";
  functionName[IsAutomorphismFcn] = "IsAutomorphism_";
  functionName[IsolatedVerticesFcn] = "IsolatedVertices_";
  functionName[IsPrimitiveElementFcn] = "IsPrimitiveElement_";
  functionName[IterateFcn] = "Iterate_";
  functionName[LengthFcn] = "Length_";
  functionName[MapFcn] = "Map_";
  functionName[MultiplyFcn] = "Multiply_";
  functionName[WhiteheadGraphFcn] = "WhiteheadGraph_";
  functionName[WhiteheadProblemFcn] = "WhiteheadProblem_";

  // place function Skeleta into list
  functionSkeleton[CommutatorFcn] = "Commutator_( u_ , v_ )";
  functionSkeleton[ComposeFcn] = "Compose_( f_ , g_ )";
  functionSkeleton[ConjugateFcn] = "Conjugate_( u_ , v_ )";
  functionSkeleton[ConjugacyProblemFcn] = "ConjugacyProblem_( u_ , v_ )";
  functionSkeleton[ConnectedComponentsFcn] = "ConnectedComponents_( Gamma_ )";
  functionSkeleton[ExpFcn] = "Exp_( u_ , n_ )";
  functionSkeleton[IdentityFcn] = "Identity_( u_ )";
  functionSkeleton[InverseFcn] = "Inverse_( u_ )";
  functionSkeleton[IsAutomorphismFcn] = "IsAutomorphism_( f_ )";
  functionSkeleton[IsolatedVerticesFcn] = "IsolatedVertices_( Gamma_ )";
  functionSkeleton[IsPrimitiveElementFcn] = "IsPrimitiveElement_( u_ , <n_> )";
  functionSkeleton[IterateFcn] = "Iterate_( f_, n_ )";
  functionSkeleton[LengthFcn] = "Length_( u_ )";
  functionSkeleton[MapFcn] = "Map_( f_ , u_ , <n_> )";
  functionSkeleton[MultiplyFcn] = "Multiply_( u_ , v_ )";
  functionSkeleton[WhiteheadGraphFcn] = "WhiteheadGraph_( u_ , <n_> )";
  functionSkeleton[WhiteheadProblemFcn] = "WhiteheadProblem_( u_, v_, <n_> )";

  // place function Status Tips into list
  functionStatusTip[CommutatorFcn] = tr(": returns uvUV");
  functionStatusTip[ComposeFcn] = tr(": returns the composition fg");
  functionStatusTip[ConjugateFcn] = tr(": returns vuV");
  functionStatusTip[ConjugacyProblemFcn] = tr(": returns a where u = avA if a exists");
  functionStatusTip[ConnectedComponentsFcn] = tr(": returns the list of the connected subgraphs of Gamma");
  functionStatusTip[ExpFcn] = tr(": returns u^n");
  functionStatusTip[IdentityFcn] = tr(": returns u");
  functionStatusTip[InverseFcn] = tr(": returns U");
  functionStatusTip[IsAutomorphismFcn] = tr(": returns the inverse of f if f is an automorphism");
  functionStatusTip[IsolatedVerticesFcn] = tr(": returns the list of isolated vertices of Gamma");
  functionStatusTip[IsPrimitiveElementFcn] = tr(": returns an automorphism f such that f(u) has length 1 if possible (n is the optional rank)");
  functionStatusTip[IterateFcn] = tr(": returns f^n");
  functionStatusTip[LengthFcn] = tr(": return the length of u");
  functionStatusTip[MapFcn] = tr(": returns f^n(u), by default n = 1");
  functionStatusTip[MultiplyFcn] = tr(": returns uv");
  functionStatusTip[WhiteheadGraphFcn] = tr(": return the Whitehead graph of u (n is the optional rank)");
  functionStatusTip[WhiteheadProblemFcn] = tr(": returns f such that f(v) = u if such an f exists (n is the optional rank)");

  // place variable types for functions into list
  functionInput[CommutatorFcn] << Element << Element;
  functionInput[ComposeFcn] << Morphism << Morphism;
  functionInput[ConjugateFcn] << Element << Element;
  functionInput[ConjugacyProblemFcn] << Element << Element;
  functionInput[ConnectedComponentsFcn] << Graph;
  functionInput[ExpFcn] << Element << Integer;
  functionInput[IdentityFcn] << Element;
  functionInput[InverseFcn] << Element;
  functionInput[IsAutomorphismFcn] << Morphism;
  functionInput[IsolatedVerticesFcn] << Graph;
  functionInput[IsPrimitiveElementFcn] << Element << OptionalInteger;
  functionInput[IterateFcn] << Morphism << Integer;
  functionInput[LengthFcn] << Element;
  functionInput[MapFcn] << Morphism << Element << OptionalInteger;
  functionInput[MultiplyFcn] << Element << Element;
  functionInput[WhiteheadGraphFcn] << Element << OptionalInteger;
  functionInput[WhiteheadProblemFcn] << Element << Element << OptionalInteger;

  // place function output into list
  functionOutput[CommutatorFcn] = Element;
  functionOutput[ComposeFcn] = Morphism;
  functionOutput[ConjugateFcn] = Element;
  functionOutput[ConjugacyProblemFcn] = Element;
  functionOutput[ConnectedComponentsFcn] = GraphList;
  functionOutput[ExpFcn] = Element;
  functionOutput[IdentityFcn] = Element;
  functionOutput[InverseFcn] = Element;
  functionOutput[IsAutomorphismFcn] = Morphism;
  functionOutput[IsolatedVerticesFcn] = StringList;
  functionOutput[IsPrimitiveElementFcn] = Morphism;
  functionOutput[IterateFcn] = Morphism;
  functionOutput[LengthFcn] = Integer;
  functionOutput[MapFcn] = Element;
  functionOutput[MultiplyFcn] = Element;
  functionOutput[WhiteheadGraphFcn] = Graph;
  functionOutput[WhiteheadProblemFcn] = Element;

}
