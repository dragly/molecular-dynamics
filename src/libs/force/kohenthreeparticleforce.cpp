#include "kohenthreeparticleforce.h"

#include "atom.h"

#include <cmath>

KohenThreeParticleForce::KohenThreeParticleForce()
{
}

void KohenThreeParticleForce::calculateAndApplyForce(Atom *atom1, Atom *atom2, Atom *atom3)
{
    calculateAndApplyForce(atom1, atom2, atom3, m_zeroVector, m_zeroVector);
}

void KohenThreeParticleForce::calculateAndApplyForce(Atom *atom1, Atom *atom2, Atom *atom3, const Vector3 &atom2Offset, const Vector3 &atom3Offset)
{
    double rc = 5.291233148782173; // 2.8 Å
    double rcSquared = rc*rc;
    double shield = 1e-12; // just to avoid nan from 0 / 0 in dtheta and acos
    double mu = 0.132587;
    double nu = 0.2997;

    rij = atom2->position() - atom1->position();
    rik = atom3->position() - atom1->position();

    double lij2 = dot(rij, rij);
    double lik2 = dot(rik, rik);
    if(lij2 > rcSquared || lik2 > rcSquared) {
        return;
    }
    double dotrijrik = dot(rij,rik);
    double lij = sqrt(lij2);
    double lik = sqrt(lik2);

    double lambda = 0.6422395192972986; // 2.80e-21 kJ
    double gamma = 2.8345891868475928; // 1.5 Å
    double cosTheta = dotrijrik / (lij * lik + shield);
    double a = 1 + mu * cosTheta + nu * cosTheta * cosTheta;
    double potential = lambda * a * exp(gamma / (lij - rc) + gamma / (lik - rc));
    atom1->addPotential(potential / 3.0);
    atom2->addPotential(potential / 3.0);
    atom3->addPotential(potential / 3.0);

    double dtheta = 0;
    double drij = 0;
    double drik = 0;

    double invLij = 1 / lij;
    double invLik = 1 / lik;
    double invSqrtDotOverLenghtSquared = 1. /
                (
                sqrt(
                    1 - (dotrijrik * dotrijrik * (invLij * invLij * invLik * invLik)) + shield
                    )
                );
    dtheta = 0;
    drij = 0;
    drik = 0;
    for(int a = 0; a < 3; a++) {
        dtheta = -(
                    ((dotrijrik * rik[a]) * (invLij * invLik * invLik * invLik))
                    + ((dotrijrik * rij[a]) * (invLij * invLij * invLij * invLik))
                    + ((-rij[a] - rik[a]) * (invLij * invLik))
                    ) * invSqrtDotOverLenghtSquared;
        drij = -rij[a] * invLij;
        drik = -rik[a] * invLik;
        double forceComp = dtheta * drij * drik * potential; // TODO This is just a dummy for benchmarking!
        if(!m_isNewtonsThirdLawEnabled) {
            forceComp *= 0.5; // We count twice if Newton's third is not enabled
        }
        atom1->addForce(a, forceComp);
    }

    dtheta = 0;
    drij = 0;
    drik = 0;
    for(int a = 0; a < 3; a++) {
        dtheta = -(
                    ((dotrijrik * (-rij[a])) * (invLij * invLij * invLij * invLik))
                    + ((rik[a]) * (invLij * invLik))
                    ) * invSqrtDotOverLenghtSquared;
        drij = rij[a] * invLij;
        double forceComp = dtheta * drij * drik * potential; // TODO This is just a dummy for benchmarking!
        if(!m_isNewtonsThirdLawEnabled) {
            forceComp *= 0.5; // We count twice if Newton's third is not enabled
        }
        atom2->addForce(a, forceComp);

    }

    dtheta = 0;
    drij = 0;
    drik = 0;
    for(int a = 0; a < 3; a++) {
        dtheta = -(
                    ((dotrijrik * (-rik[a])) * (invLij * invLik * invLik * invLik))
                    + ((rij[a]) * (invLij * invLik))
                    ) * invSqrtDotOverLenghtSquared;
        drik = rik[a] * invLik;
        double forceComp = dtheta * drij * drik * potential; // TODO This is just a dummy for benchmarking!
        if(!m_isNewtonsThirdLawEnabled) {
            forceComp *= 0.5; // We count twice if Newton's third is not enabled
        }
        atom3->addForce(a,forceComp);
    }
}
