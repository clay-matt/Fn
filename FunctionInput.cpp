//

#include "FunctionInput.h"

/////////////////////////////////////////////////////////

FunctionInput::FunctionInput() :
    QList<FnData>()
{

}

/////////////////////////////////////////////////////////

bool FunctionInput::isAcceptable(const QList<enum VariableTypes> & types) const
{

    if(types.size() < 2*size()) return false;

    for (int i = 0; 2*i < types.size(); i++) {
        if (i < size()) {
            if (at(i).type() != types.at(2*i)) {
                if (optional(at(i).type()) != types.at(2*i)) return false;
            }
            if (at(i).isList()) {
                if (types.at(2*i + 1) != Listable) return false;
            }
        } else {
            if (types.at(2*i) != OptionalInteger) return false;
        }
    }

  return true;
}

/////////////////////////////////////////////////////////

QList<enum VariableTypes> FunctionInput::varTypes() const
{

  QList<enum VariableTypes> types;
  QListIterator<FnData> i(*this);
  while (i.hasNext())
    types.append(i.next().type());

  return types;

}

/////////////////////////////////////////////////////////

enum VariableTypes FunctionInput::optional(const enum VariableTypes type) const
{

  if (type == Integer) return OptionalInteger;

  return NoType;

}

/////////////////////////////////////////////////////////
