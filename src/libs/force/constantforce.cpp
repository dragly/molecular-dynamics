#include "constantforce.h"

#include <atom.h>

ConstantForce::ConstantForce()
{
}

void ConstantForce::apply(Atom *atom)
{
    if(atom->isPositionFixed()) {
        return;
    }
    atom->addForce(m_forceVector);
}
