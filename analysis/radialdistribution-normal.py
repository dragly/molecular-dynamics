# -*- coding: utf-8 -*-
"""
Created on Tue Feb 26 08:54:28 2013

@author: svenni
"""

from pylab import *
from sys import argv
from glob import glob
from time import time
from fys4460 import loadAtoms, boltzmannConstant
from os.path import expanduser, join, split, isdir, islink
from os import readlink
from pylibconfig import Config
import os
import subprocess
#import os
#import h5py
configFilePaths = argv[1:]
combinations = [["8","8"],
                ["8","14"],
                ["14","14"]]
for configFilePath in configFilePaths:
    print "Loading config file"
    configFilePath
    
    if islink(configFilePath):
        configFilePath = readlink(configFilePath)
        
    saveDir, configFileName = split(configFilePath)
    
    config = Config()
    config.readFile(configFilePath)
    fileNames = config.value("simulation.saveFileName")[0]
    fileNames = expanduser(fileNames)
    fileNames = glob(fileNames)
    
    fileNames.sort()
    
    for combination in combinations:
        distBins = [0,1000]
        totalBins = zeros(distBins[1])
        iFiles = 0
        temperature = 0
        
        for fileName in fileNames[-5:-1]:
            header, lammps, atoms = loadAtoms(fileName)
            temperature += header["temperature"]
            outFileName = fileName.replace("data", "distances")
            process = subprocess.call(["../tools/radial-distribution-build-Desktop_Qt_5_0_1_GCC_64bit-Release/radial-distribution", fileName, outFileName] + combination)
            outFile = open(outFileName, "rb")
            nBins = fromfile(outFile, dtype='int32', count=1)
            binEdges = fromfile(outFile, dtype=float, count=nBins + 1) 
            binContents = fromfile(outFile, dtype=float, count=nBins)
            outFile.close()
    #        distances = fromfile(outFileName)
            os.remove(outFileName)
            
            #figure("histFigure")
            #distBins = hist(distances, bins=distBins[1])
            V = 4./3. * pi * binEdges**3
            Vdiff = V[1:] - V[:-1]
            newBins = binContents / (Vdiff * sum(binContents))
            totalBins = totalBins + newBins
                
            iFiles += 1
        temperature /= iFiles
        temperatureLabel = "%.0f K" % temperature
        totalBins /= iFiles
#        latestFigure = figure("latestFigure")
#        plot(binEdges[:-1] * 1e10, newBins, label=temperatureLabel)
#        xlabel(u"r [Å]")
#        ylabel("g(r)")
#        legend()
        figure()
        title("-".join(combination))
        plot(binEdges[:-1] * 1e10, totalBins, label=temperatureLabel)
        xlabel(u"r [Å]")
        ylabel("g(r)")
        xlim(0,12)
        legend()
        numberDensity = len(atoms) / prod(header["upperBounds"] - header["lowerBounds"])
        totalMass = sum(atoms["type"] == 8)*2.65676264126474e-26  + sum(atoms["type"] == 14) * 4.66370658657455e-26
        grid()
        print "Mass density (g/cm^3): " + str(totalMass * 1e3 / (prod(header["upperBounds"] - header["lowerBounds"]) * 1e6))
        
#        latestFigure.savefig(join(saveDir, "radial-distribution-latest.pdf"))
        savefig(join(saveDir, "radial-distribution-" + "-".join(combination) + ".pdf"))
show()