#include <src/molecule.h>
#include <src/atom.h>

#include <src/moleculesystemcell.h>
#include <src/moleculesystem.h>
#include <src/interatomicforce.h>

MoleculeSystemCell::MoleculeSystemCell(MoleculeSystem *parent) :
    m_nDimensions(3),
    pow3nDimensions(pow(3, m_nDimensions)),
    m_indices(zeros<irowvec>(m_nDimensions)),
    moleculeSystem(parent),
    m_hasAlreadyCalculatedForcesBetweenSelfAndNeighbors(false),
    m_id(0),
    force(blankForce)
{
    cellShiftVectors = zeros(pow3nDimensions, m_nDimensions);
}

void MoleculeSystemCell::setBoundaries(mat boundaries)
{
    m_boundaries = boundaries;
    irowvec counters = zeros<irowvec>(m_nDimensions);
    for(int i = 0; i < pow3nDimensions; i++) {
        irowvec direction = counters - ones<irowvec>(m_nDimensions);
        rowvec lengths = m_boundaries.row(1) - m_boundaries.row(0);
        rowvec directionVec = conv_to<rowvec>::from(direction);
        cellShiftVectors.row(i) = lengths % directionVec;
        counters(0) += 1;
        for(uint idim = 1; idim < counters.size(); idim++) {
            if(counters(idim - 1) > 2) {
                counters(idim - 1) = 0;
                counters(idim) += 1;
            }
        }
    }
    //    cout << cellShiftVectors << endl;
}

void MoleculeSystemCell::addNeighbor(MoleculeSystemCell *cell, const rowvec &offset)
{
    m_neighborCells.push_back(cell);
    m_neighborOffsets.push_back(offset);
}

const mat &MoleculeSystemCell::boundaries() const
{
    return m_boundaries;
}

ostream& operator<<(ostream& os, MoleculeSystemCell* dt)
{
    os << dt->m_boundaries << endl;
    os << dt->m_indices << endl;
    return os;
}

ostream& operator<<(ostream& os, const MoleculeSystemCell& dt)
{
    os << dt.m_boundaries << endl;
    os << dt.m_indices << endl;
    return os;
}

void MoleculeSystemCell::addMolecule(Molecule *molecule) {
    m_molecules.push_back(molecule);
    for(Atom* atom : molecule->atoms()) {
        m_atoms.push_back(atom);
    }
}

const vector<Atom *> &MoleculeSystemCell::atoms()
{
    return m_atoms;
}

const vector<Molecule *> &MoleculeSystemCell::molecules()
{
    return m_molecules;
}

void MoleculeSystemCell::setIndices(const irowvec& indices)
{
    m_indices = indices;
}

const irowvec &MoleculeSystemCell::indices() const
{
    return m_indices;
}


void MoleculeSystemCell::updateForces()
{
    Atom* atom1;
    Atom* atom2;
    for(uint iNeighbor = 0; iNeighbor < m_neighborCells.size(); iNeighbor++) {
        MoleculeSystemCell* neighbor = m_neighborCells.at(iNeighbor);
        rowvec& neighborOffset = m_neighborOffsets.at(iNeighbor);
        if(neighbor->hasAlreadyCalculatedForcesBetweenSelfAndNeighbors()) {
            continue;
        }
        for(uint iAtom = 0; iAtom < m_atoms.size(); iAtom++) {
            atom1 = m_atoms.at(iAtom);
            for(uint jAtom = 0; jAtom < neighbor->atoms().size(); jAtom++) {
                atom2 = neighbor->atoms().at(jAtom);
                moleculeSystem->interatomicForce()->calculateAndApplyForce(atom1, atom2, neighborOffset);
//                force = moleculeSystem->interatomicForce()->force();
//                double potential = moleculeSystem->interatomicForce()->potential();
//                atom2->addForce(-force);
//                atom1->addForce(force);
//                atom2->addPotential(0.5 * potential);
//                atom1->addPotential(0.5 * potential);
            }
        }
        m_hasAlreadyCalculatedForcesBetweenSelfAndNeighbors = true;
    }

    // Loop over own atoms
    for(uint iAtom = 0; iAtom < m_atoms.size(); iAtom++) {
        atom1 = m_atoms.at(iAtom);
        // Loop over all neighbors (including ourselves)
        for(uint jAtom = iAtom + 1; jAtom < m_atoms.size(); jAtom++) {
            atom2 = m_atoms.at(jAtom);
            moleculeSystem->interatomicForce()->calculateAndApplyForce(atom1, atom2);
//            force = moleculeSystem->interatomicForce()->force();
//            double potential = moleculeSystem->interatomicForce()->potential();
//            atom2->addForce(-force);
//            atom1->addForce(force);
//            atom2->addPotential(0.5 * potential);
//            atom1->addPotential(0.5 * potential);
        }
    }
}

void MoleculeSystemCell::clearMolecules()
{
    m_molecules.clear();
    m_atoms.clear();
}


//void MoleculeSystemCell::addAlreadyCalculatedNeighbor(MoleculeSystemCell *neighbor, const rowvec& offset)
//{
//    m_alreadyCalculatedNeighbors.push_back(neighbor);
//    m_neighborWithAlreadyCalculatedForcesOffsets.push_back(offset);
//}

void MoleculeSystemCell::clearAlreadyCalculatedNeighbors()
{
//    m_alreadyCalculatedNeighbors.clear();
    m_hasAlreadyCalculatedForcesBetweenSelfAndNeighbors = false;
}

bool MoleculeSystemCell::hasAlreadyCalculatedForcesBetweenSelfAndNeighbors()
{
    return m_hasAlreadyCalculatedForcesBetweenSelfAndNeighbors;
}

void MoleculeSystemCell::setID(int id)
{
    m_id = id;
}

int MoleculeSystemCell::id()
{
    return m_id;
}
