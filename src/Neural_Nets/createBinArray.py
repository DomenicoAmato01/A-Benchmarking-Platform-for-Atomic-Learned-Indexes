import argparse
import json
import pickle
import os
import h5py
import tensorflow as tf
import numpy as np
from numpy import genfromtxt



#Set Argument Parser for input information
parser = argparse.ArgumentParser(description='Train NN for Datastructure index')
parser = argparse.ArgumentParser(description='Train NN for Datastructure index')
parser.add_argument('-i', '--input', dest="inputFile", default="uni01.sorted.csv",
                    help='Input file name')
parser.add_argument('-id', '--inputDir', dest="inputDir", default="rsc/uniform/",
help='Input file path')
parser.add_argument('-o', '--output', dest="outputPath", default="rsc/uniform/uni01.sorted.bin.h5",
                    help='Output file path and file name')

args = parser.parse_args()

my_data = genfromtxt(os.path.join(args.inputDir, args.inputFile), delimiter=',', dtype=int)
print(my_data)

binData=[]
for data in my_data:
    binData.append([int(x) for x in list('{0:064b}'.format(data))])

with h5py.File(args.outputPath, "w") as f:
    f.create_dataset("Sb", data=binData)

print(binData)
