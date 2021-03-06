TEMPLATE = lib
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include(../../defaults.pri)

mpi {
    TARGET = emdeempi
} else:android {
    TARGET = emdeeandroid
} else {
    TARGET = emdee
}

SOURCES += \
    moleculesystem.cpp \
    generator.cpp \
    atomtype.cpp \
    moleculesystemcell.cpp \
    integrator/velocityverletintegrator.cpp \
    integrator/integrator.cpp \
    integrator/eulercromerintegrator.cpp \
    modifier/modifier.cpp \
    modifier/berendsenthermostat.cpp \
    modifier/andersenthermostat.cpp \
    random.cpp \
    atom.cpp \
    force/lennardjonesforce.cpp \
    force/twoparticleforce.cpp \
    math/vector3.cpp \
    range.cpp \
    force/singleparticleforce.cpp \
    force/constantforce.cpp \
    force/vashishtatwoparticleforce.cpp \
    force/vashishtathreeparticleforce.cpp \
    force/threeparticleforce.cpp \
    force/fanntwoparticleforce.cpp \
    force/fannthreeparticleforce.cpp \
    modifier/friction.cpp \
    utils/fannderivative.cpp \
    utils/setup.cpp \
    force/kohenthreeparticleforce.cpp

HEADERS += \
    moleculesystem.h \
    generator.h \
    atomtype.h \
    moleculesystemcell.h \
    integrator/velocityverletintegrator.h \
    integrator/integrator.h \
    integrator/eulercromerintegrator.h \
    modifier/modifier.h \
    modifier/berendsenthermostat.h \
    modifier/andersenthermostat.h \
    random.h \
    atom.h \
    force/lennardjonesforce.h \
    force/twoparticleforce.h \
    math/vector3.h \
    range.h \
    progressreporter.h \
    force/singleparticleforce.h \
    force/constantforce.h \
    force/vashishtatwoparticleforce.h \
    force/vashishtathreeparticleforce.h \
    force/threeparticleforce.h \
    force/fanntwoparticleforce.h \
    utils/logging.h \
    utils/glogfallback.h \
    force/fannthreeparticleforce.h \
    modifier/friction.h \
    utils/fannderivative.h \
    utils/setup.h \
    force/kohenthreeparticleforce.h

!nofann {
    HEADERS +=
    SOURCES +=
    LIBS += -ldoublefann
}

mpi {
    SOURCES += \
        processor_mpi.cpp \
        filemanager.cpp
    HEADERS +=\
        processor.h \
        filemanager.h
} else {
    SOURCES += \
        processor_nompi.cpp
    HEADERS +=\
        processor.h
}
