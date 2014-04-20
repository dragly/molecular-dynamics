#include <unittest++/UnitTest++.h>

#include <moleculesystem.h>
#include <moleculesystemcell.h>
#include <generator.h>
#include <atomtype.h>
#include <force/lennardjonesforce.h>
#include <force/fannthreeparticleforce.h>
#include <force/fanntwoparticleforce.h>
#include <modifier/andersenthermostat.h>
#include <modifier/berendsenthermostat.h>
#include <atom.h>
#include <processor.h>
#include <utils/logging.h>
#include <force/threeparticleforce.h>
#include <integrator/velocityverletintegrator.h>
#include <filemanager.h>

SUITE(FannForceSystem) {
    TEST(Dummy) {

    }

    //    TEST(FannFunTest) {
    //        // Particle types
    //        AtomType hydrogenType(0);
    //        hydrogenType.setMass(1.0);
    //        hydrogenType.setNumber(1);
    //        hydrogenType.setAbbreviation("H");
    //        ofstream outfile("data.out");
    //        for(int i = 0; i < 100; i++) {
    //            Atom *hydrogenAtom1 = new Atom(hydrogenType);
    //            hydrogenAtom1->setPosition(Vector3(0,0,0));
    //            Atom *hydrogenAtom2 = new Atom(hydrogenType);
    //            hydrogenAtom2->setPosition(Vector3(0,1.0/1.89+ double(i)/100.0 * 6/1.89,0));
    //            FannTwoParticleForce testForce2;
    //            testForce2.setNewtonsThirdLawEnabled(true);
    //            testForce2.loadNetwork("/home/svenni/Dropbox/studies/master/results/fann_train/20140418-165800/fann_network.net",
    //                                   "/home/svenni/Dropbox/studies/master/results/fann_train/20140418-165800/bounds.fann");
    //            testForce2.setCutoffRadius(6.0 / 1.89);
    //            testForce2.calculateAndApplyForce(hydrogenAtom1, hydrogenAtom2);
    //            outfile << hydrogenAtom2->position().y()*1.8897 << " " << hydrogenAtom1->force().y() << endl;
    //        }
    //        outfile.close();
    //    }

    TEST(FannForceWater)
    {
        LOG(INFO) << "FannForceWater test started";

        // Particle types
        AtomType hydrogenType(0);
        hydrogenType.setMass(1.0);
        hydrogenType.setNumber(1);
        hydrogenType.setAbbreviation("H");

        AtomType oxygenType(1);
        oxygenType.setMass(15.9994);
        oxygenType.setNumber(8);
        oxygenType.setAbbreviation("O");

        vector<AtomType> particleTypes;
        particleTypes.push_back(hydrogenType);
        particleTypes.push_back(oxygenType);

        vector<Atom *> atoms;

        double sideLength = 9.4;

        int type = 2;
        if(type == 0)  {
            int nx = 3;
            int ny = 3;
            int nz = 3;
            double spacingx = sideLength / nx;
            double spacingy = sideLength / ny;
            double spacingz = sideLength / nz;
            int idCounter = 0;
            for(int i = 0; i < nz; i++) {
                for (int j = 0; j < ny; j++) {
                    for (int k = 0; k < nx; ++k) {
                        Atom *hydrogenAtom1 = new Atom(hydrogenType);
                        hydrogenAtom1->setPosition(Vector3(-0.7575 + spacingx*k, 0.58707 + spacingy*j, spacingz*i));
                        //            hydrogenAtom1->setPosition(Vector3(-0.5, 0.58707, i));
                        hydrogenAtom1->setID(1 + idCounter);
                        Atom *hydrogenAtom2 = new Atom(hydrogenType);
                        hydrogenAtom2->setPosition(Vector3(0.7575 + spacingx*k, 0.58707 + spacingy*j, spacingz*i));
                        //            hydrogenAtom2->setPosition(Vector3(0.5, 0.58707, i));
                        hydrogenAtom2->setID(2 + idCounter);
                        Atom *oxygenAtom = new Atom(oxygenType);
                        oxygenAtom->setPosition(Vector3(0.0 + spacingx*k, 0.0 + spacingy*j, spacingz*i));
                        oxygenAtom->setID(3 + idCounter);
                        atoms.push_back(hydrogenAtom1);
                        atoms.push_back(hydrogenAtom2);
                        atoms.push_back(oxygenAtom);
                        idCounter += 3;
                    }
                }
            }
        } else if(type == 1) {
            Atom *hydrogenAtom1 = new Atom(hydrogenType);
            hydrogenAtom1->setPosition(Vector3(-0.7575, 0.58707, 0));
            hydrogenAtom1->setID(1);
            Atom *hydrogenAtom2 = new Atom(hydrogenType);
            hydrogenAtom2->setPosition(Vector3(0.7575, 0.58707, 0));
            //            hydrogenAtom2->setPosition(Vector3(0.5, 0.58707, i));
            hydrogenAtom2->setID(2);
            Atom *oxygenAtom = new Atom(oxygenType);
            oxygenAtom->setPosition(Vector3(0.0, 0.0, 0));
            oxygenAtom->setID(3);
            atoms.push_back(hydrogenAtom1);
            atoms.push_back(hydrogenAtom2);
            atoms.push_back(oxygenAtom);

//            // Move molecule to center
//            for(Atom *atom : atoms) {
//                atom->setPosition(atom->position() + Vector3(sideLength / 2.0, sideLength / 2.0, sideLength / 2.0));
//            }
        } else if(type == 2) {
            Atom *hydrogenAtom1 = new Atom(hydrogenType);
            hydrogenAtom1->setPosition(Vector3(-0.7575, 1-0.58707, 0));
            hydrogenAtom1->setID(1);
            Atom *hydrogenAtom2 = new Atom(hydrogenType);
            hydrogenAtom2->setPosition(Vector3(0.7575, 1-0.58707, 0));
            //            hydrogenAtom2->setPosition(Vector3(0.5, 0.58707, i));
            hydrogenAtom2->setID(2);
            Atom *oxygenAtom = new Atom(oxygenType);
            oxygenAtom->setPosition(Vector3(0.0, 1+0.0, 0));
            oxygenAtom->setID(3);
            double distance = 2.0;
            Atom *hydrogenAtom3 = new Atom(hydrogenType);
            hydrogenAtom3->setPosition(Vector3(-0.7575, distance+0.58707, 0));
            hydrogenAtom3->setID(4);
            Atom *hydrogenAtom4 = new Atom(hydrogenType);
            hydrogenAtom4->setPosition(Vector3(0.7575, distance+0.58707, 0));
            hydrogenAtom4->setID(5);
            Atom *oxygenAtom2 = new Atom(oxygenType);
            oxygenAtom2->setPosition(Vector3(0.0, distance+0.0, 0));
            oxygenAtom2->setID(6);
            atoms.push_back(hydrogenAtom1);
            atoms.push_back(hydrogenAtom2);
            atoms.push_back(oxygenAtom);
            atoms.push_back(hydrogenAtom3);
            atoms.push_back(hydrogenAtom4);
            atoms.push_back(oxygenAtom2);
        }

        bool thermo = false;
        if(thermo) {
            Generator gen;
            gen.boltzmannDistributeVelocities(0.001, atoms);
        }

        MoleculeSystem system;
        system.setPeriodicity(true, true, true);
        system.setBoundaries(0.0, sideLength, 0.0, sideLength, 0.0, sideLength);
        system.setParticleTypes(particleTypes);
        system.addAtoms(atoms);

        FannTwoParticleForce testForce2;
        testForce2.addNetwork(hydrogenType, hydrogenType,
                              "/home/svenni/Dropbox/studies/master/results/fann_train/20140419-164116/fann_network.net",
                              "/home/svenni/Dropbox/studies/master/results/fann_train/20140419-164116/bounds.fann");
        testForce2.addNetwork(hydrogenType, oxygenType,
                              "/home/svenni/Dropbox/studies/master/results/fann_train/20140418-165017/fann_network.net",
                              "/home/svenni/Dropbox/studies/master/results/fann_train/20140418-165017/bounds.fann");
        testForce2.addNetwork(oxygenType, oxygenType,
                              "/home/svenni/Dropbox/studies/master/results/fann_train/20140418-162313/fann_network.net",
                              "/home/svenni/Dropbox/studies/master/results/fann_train/20140418-162313/bounds.fann");

        //        LennardJonesForce testForce2;
        //        testForce2.setPotentialConstant(pow(2, -1.0/6.0) * 5 / 1.89);
        //        testForce2.setShift(3.6 / 1.89);
        //        testForce2.setEnergyConstant(1);

        testForce2.setCutoffRadius(sideLength / 3);

        cout << "Cutoff: " << testForce2.cutoffRadius() << endl;

        system.setTwoParticleForce(&testForce2);

        FannThreeParticleForce testForce3;
        bool flat = false;
        if(flat) {
            testForce3.loadNetwork("/home/svenni/Dropbox/studies/master/results/fann_train/20140420-220410/fann_network.net",
                                   "/home/svenni/Dropbox/studies/master/results/fann_train/20140420-220410/bounds.fann");
        } else {
            testForce3.loadNetwork("/home/svenni/Dropbox/studies/master/results/fann_train/20140420-001516/fann_network.net",
                                   "/home/svenni/Dropbox/studies/master/results/fann_train/20140420-001516/bounds.fann");
        }
        system.setThreeParticleForce(&testForce3);

        VelocityVerletIntegrator integrator(&system);
        integrator.setTimeStep(0.01);
        system.setIntegrator(&integrator);

        BerendsenThermostat thermostat(&system);
        if(thermo) {
            system.addModifier(&thermostat);
        }

        FileManager fileManager(&system);
        fileManager.setOutFileName("/tmp/fannforce/atoms*.xyz");
        fileManager.setUnitLength(1.0e-10);
        system.setFileManager(&fileManager);

        system.setSaveEnabled(true);
        system.setSaveEveryNSteps(100);
        system.setOutputEnabled(true);

        system.setupCells();


        if(thermo) {
            system.setNSimulationSteps(10000);

            thermostat.setTargetTemperature(0.01);
            system.simulate();
            thermostat.setTargetTemperature(0.005);
            system.simulate();
            thermostat.setTargetTemperature(0.001);
            system.simulate();
            thermostat.setTargetTemperature(0.0001);
            system.simulate();
            system.removeModifier(&thermostat);
            system.simulate();
        } else {
            system.setNSimulationSteps(100000);
            system.simulate();
        }

        //        CHECK_EQUAL(3, system.nAtomsTotal());

        LOG(INFO) << "FannForceWater test complete";
    }
}
