#include "atom.h"

//void Atom::


//void Atom::

void Atom::clone(const Atom &other)
{
    this->m_id = other.m_id;
    this->m_position = other.m_position;
    this->m_cellID = other.m_cellID;
    this->m_displacement = other.m_displacement;
    this->m_force = other.m_force;
    this->m_localPressure = other.m_localPressure;
    this->m_potential = other.m_potential;
    this->m_atomType = other.m_atomType;
    this->m_velocity = other.m_velocity;
}

//void Atom::

//void Atom::

//void Atom::

//void Atom::

//void Atom::

//void Atom::

//void Atom::

//template<class Archive>
//void Atom::serialize(Archive & ar, const unsigned int version)
//{
//    ar & m_position;
//    ar & m_velocity;
//    ar & m_force;
//    ar & m_displacement;
//    ar & m_mass;
//    ar & m_potential;
//    ar & m_localPressure;
//    ar & m_cellID;
//    ar & m_type;
//}