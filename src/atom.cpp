#include <src/atom.h>

Atom::Atom(AtomType atomType) :
    m_mass(0.0),
    m_potential(0.0),
    m_localPressure(0.0),
    m_cellID(-999),
    m_type(atomType)
{
}

//void Atom::


//void Atom::

void Atom::clearForcePotentialPressure()
{
    m_force.zeros();
    m_potential = 0;
    m_localPressure = 0;
}

void Atom::clearDisplacement() {
    m_displacement.zeros();
}

//void Atom::

//void Atom::

//void Atom::

//void Atom::

//void Atom::

//void Atom::

//void Atom::
