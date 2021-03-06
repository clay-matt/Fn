//

#include "Fn.h"
#include "FunctionInfo.h"

FunctionInfo::FunctionInfo(QObject *parent) :
    QObject(parent)
{

  // place function tags into list
  functionEnum["Abelianization_"] = AbelianizationFcn;
  functionEnum["Bicomponents_"] = BicomponentsFcn;
  functionEnum["Commutator_"] = CommutatorFcn;
  functionEnum["Compose_"] = ComposeFcn;
  functionEnum["Conjugate_"] = ConjugateFcn;
  functionEnum["ConjugacyProblem_"] = ConjugacyProblemFcn;
  functionEnum["ConnectedComponents_"] = ConnectedComponentsFcn;
  functionEnum["Exp_"] = ExpFcn;
  functionEnum["Identity_"] = IdentityFcn;
  functionEnum["Inverse_"] = InverseFcn;
  functionEnum["IsAutomorphism_"] = IsAutomorphismFcn;
  functionEnum["IsPrimitiveElement_"] = IsPrimitiveElementFcn;
  functionEnum["IsSeparable_"] = IsSeparableFcn;
  functionEnum["Iterate_"] = IterateFcn;
  functionEnum["Length_"] = LengthFcn;
  functionEnum["Map_"] = MapFcn;
  functionEnum["Multiply_"] = MultiplyFcn;
  functionEnum["TwoStepNilpotent_"] = TwoStepNilpotentFcn;
  functionEnum["WhiteheadAutomorphism_"] = WhiteheadAutomorphismFcn;
  functionEnum["WhiteheadGraph_"] = WhiteheadGraphFcn;
  functionEnum["WhiteheadProblem_"] = WhiteheadProblemFcn;

  // place function names into list
  functionName[AbelianizationFcn] = "Abelianization_";
  functionName[BicomponentsFcn] = "Bicomponents_";
  functionName[CommutatorFcn] = "Commutator_";
  functionName[ComposeFcn] = "Compose_";
  functionName[ConjugateFcn] = "Conjugate_";
  functionName[ConjugacyProblemFcn] = "ConjugacyProblem_";
  functionName[ConnectedComponentsFcn] = "ConnectedComponents_";
  functionName[ExpFcn] = "Exp_";
  functionName[IdentityFcn] = "Identity_";
  functionName[InverseFcn] = "Inverse_";
  functionName[IsAutomorphismFcn] = "IsAutomorphism_";
  functionName[IsPrimitiveElementFcn] = "IsPrimitiveElement_";
  functionName[IsSeparableFcn] = "IsSeparable_";
  functionName[IterateFcn] = "Iterate_";
  functionName[LengthFcn] = "Length_";
  functionName[MapFcn] = "Map_";
  functionName[MultiplyFcn] = "Multiply_";
  functionName[TwoStepNilpotentFcn] = "TwoStepNilpotent_";
  functionName[WhiteheadAutomorphismFcn] = "WhiteheadAutomorphism_";
  functionName[WhiteheadGraphFcn] = "WhiteheadGraph_";
  functionName[WhiteheadProblemFcn] = "WhiteheadProblem_";

  // place function Skeleta into list
  functionSkeleton[AbelianizationFcn] = "Abelianization_( u_ , <n_> )";
  functionSkeleton[BicomponentsFcn] = "Bicomponents_( Gamma_ )";
  functionSkeleton[CommutatorFcn] = "Commutator_( u_ , v_ )";
  functionSkeleton[ComposeFcn] = "Compose_( f_ , g_ )";
  functionSkeleton[ConjugateFcn] = "Conjugate_( [u_] , v_ )";
  functionSkeleton[ConjugacyProblemFcn] = "ConjugacyProblem_( u_ , v_ )";
  functionSkeleton[ConnectedComponentsFcn] = "ConnectedComponents_( Gamma_ )";
  functionSkeleton[ExpFcn] = "Exp_( [u_] , n_ )";
  functionSkeleton[IdentityFcn] = "Identity_( [u_] )";
  functionSkeleton[InverseFcn] = "Inverse_( [u_] )";
  functionSkeleton[IsAutomorphismFcn] = "IsAutomorphism_( f_ )";
  functionSkeleton[IsPrimitiveElementFcn] = "IsPrimitiveElement_( u_ , <n_> )";
  functionSkeleton[IsSeparableFcn] = "IsSeparable_( [u_] , n_ )";
  functionSkeleton[IterateFcn] = "Iterate_( f_, n_ )";
  functionSkeleton[LengthFcn] = "Length_( [u_] )";
  functionSkeleton[MapFcn] = "Map_( f_ , [u_] , <n_> )";
  functionSkeleton[MultiplyFcn] = "Multiply_( u_ , v_ )";
  functionSkeleton[TwoStepNilpotentFcn] = "TwoStepNilpotent_( u_, <n_> )";
  functionSkeleton[WhiteheadAutomorphismFcn] = "WhiteheadAutomorphism_( [A_] , a_ , n_ )";
  functionSkeleton[WhiteheadGraphFcn] = "WhiteheadGraph_( [u_] , <n_> )";
  functionSkeleton[WhiteheadProblemFcn] = "WhiteheadProblem_( u_, v_, <n_> )";

  // place function Status Tips into list
  functionStatusTip[AbelianizationFcn] = tr(": returns the abelianization of u (n is the optional rank)");
  functionStatusTip[BicomponentsFcn] = tr(": returns the bicomponents of Gamma");
  functionStatusTip[CommutatorFcn] = tr(": returns uvUV");
  functionStatusTip[ComposeFcn] = tr(": returns the composition fg");
  functionStatusTip[ConjugateFcn] = tr(": returns vuV");
  functionStatusTip[ConjugacyProblemFcn] = tr(": returns a where u = avA if a exists");
  functionStatusTip[ConnectedComponentsFcn] = tr(": returns the list of the connected subgraphs of Gamma");
  functionStatusTip[ExpFcn] = tr(": returns u^n");
  functionStatusTip[IdentityFcn] = tr(": returns u");
  functionStatusTip[InverseFcn] = tr(": returns U");
  functionStatusTip[IsAutomorphismFcn] = tr(": returns the inverse of f if f is an automorphism");
  functionStatusTip[IsPrimitiveElementFcn] = tr(": returns an automorphism f such that f(u) has length 1 if possible (n is the optional rank)");
  functionStatusTip[IsSeparableFcn] = tr(": returns whether or not the set of words [u_] in rank n is separable");
  functionStatusTip[IterateFcn] = tr(": returns f^n");
  functionStatusTip[LengthFcn] = tr(": return the length of u");
  functionStatusTip[MapFcn] = tr(": returns f^n(u), by default n = 1");
  functionStatusTip[MultiplyFcn] = tr(": returns uv");
  functionStatusTip[TwoStepNilpotentFcn] = tr(": return the 2-step nilpotent normal form of u (n is the optional rank)");
  functionStatusTip[WhiteheadAutomorphismFcn] = tr(": returns the Whitehead Automorphism (A,a) in rank n");
  functionStatusTip[WhiteheadGraphFcn] = tr(": return the Whitehead graph of u (n is the optional rank)");
  functionStatusTip[WhiteheadProblemFcn] = tr(": returns f such that f(v) = u if such an f exists (n is the optional rank)");

  // place variable types for functions into list
  functionInput[AbelianizationFcn] << Element << NotListable << OptionalInteger << NotListable;
  functionInput[BicomponentsFcn] << Graph << NotListable;
  functionInput[CommutatorFcn] << Element << NotListable << Element << NotListable;
  functionInput[ComposeFcn] << Morphism << NotListable << Morphism << NotListable;
  functionInput[ConjugateFcn] << Element << Listable << Element << NotListable;
  functionInput[ConjugacyProblemFcn] << Element << NotListable << Element << NotListable;
  functionInput[ConnectedComponentsFcn] << Graph << NotListable;
  functionInput[ExpFcn] << Element << Listable << Integer << NotListable;
  functionInput[IdentityFcn] << Element << Listable;
  functionInput[InverseFcn] << Element << Listable;
  functionInput[IsAutomorphismFcn] << Morphism << NotListable;
  functionInput[IsPrimitiveElementFcn] << Element << NotListable << OptionalInteger << NotListable;
  functionInput[IsSeparableFcn] << Element << Listable << Integer << NotListable;
  functionInput[IterateFcn] << Morphism << NotListable << Integer << NotListable;
  functionInput[LengthFcn] << Element << Listable;
  functionInput[MapFcn] << Morphism << NotListable << Element << Listable << OptionalInteger << NotListable;
  functionInput[MultiplyFcn] << Element << NotListable << Element << NotListable;
  functionInput[TwoStepNilpotentFcn] << Element << NotListable << OptionalInteger << NotListable;
  functionInput[WhiteheadAutomorphismFcn] << Element << Listable << Element << NotListable << Integer << NotListable;
  functionInput[WhiteheadGraphFcn] << Element << Listable << OptionalInteger << NotListable;
  functionInput[WhiteheadProblemFcn] << Element << NotListable << Element << NotListable << OptionalInteger << NotListable;

  // place function output into list
  functionOutput[AbelianizationFcn] = Integer;
  functionOutput[BicomponentsFcn] = Graph;
  functionOutput[CommutatorFcn] = Element;
  functionOutput[ComposeFcn] = Morphism;
  functionOutput[ConjugateFcn] = Element;
  functionOutput[ConjugacyProblemFcn] = Element;
  functionOutput[ConnectedComponentsFcn] = Graph;
  functionOutput[ExpFcn] = Element;
  functionOutput[IdentityFcn] = Element;
  functionOutput[InverseFcn] = Element;
  functionOutput[IsAutomorphismFcn] = Morphism;
  functionOutput[IsPrimitiveElementFcn] = Morphism;
  functionOutput[IsSeparableFcn] = Morphism;
  functionOutput[IterateFcn] = Morphism;
  functionOutput[LengthFcn] = Integer;
  functionOutput[MapFcn] = Element;
  functionOutput[MultiplyFcn] = Element;
  functionOutput[TwoStepNilpotentFcn] = Integer;
  functionOutput[WhiteheadAutomorphismFcn] = Morphism;
  functionOutput[WhiteheadGraphFcn] = Graph;
  functionOutput[WhiteheadProblemFcn] = Element;

}
