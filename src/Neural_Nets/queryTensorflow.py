import argparse
import json
import pickle
import os
import h5py
import tensorflow as tf
import numpy as np
import time


def mean_error(set_dim):
    def mean_error_metric(y_true, y_pred):
        dim = tf.constant(set_dim, dtype=tf.float32)
        real = tf.math.scalar_mul(dim, y_true)
        pred = tf.math.scalar_mul(dim, y_pred)
        mean_err = tf.math.reduce_mean(tf.math.abs(tf.math.subtract(real,pred)))
        return mean_err

    return mean_error_metric
        
def max_error(set_dim):
    def max_error_metric(y_true, y_pred):
        dim = tf.constant(set_dim, dtype=tf.float32)
        #y_pred = tf.nn.relu(y_pred)
        real = tf.math.ceil(tf.math.scalar_mul(dim, y_true))
        pred = tf.math.ceil(tf.math.scalar_mul(dim, y_pred))
        pred = tf.nn.relu(pred)
        max_err = tf.math.reduce_max(tf.math.abs(tf.math.subtract(real,pred)))
        return max_err

    return max_error_metric


#Set Argument Parser for input information
parser = argparse.ArgumentParser(description='Train NN for Datastructure index')
parser.add_argument('-i', '--input', dest="inputFile", default="uni01.Query10.bin.h5",
                    help='Input file name')
parser.add_argument('-id', '--inputDir', dest="inputDir", default="rsc/uniform/Query",
help='Input file path')
parser.add_argument('-o', '--output', dest="outputPath", default="res/uniform/",
                    help='Output file path')
parser.add_argument('-m', '--model', dest="models", nargs="+", default=[1], type=int, choices=range(1, 4),
                    help='Model numbers: 1 -> No hidden Layer(Perceptron); 2 -> One hidden layer with 256 units; 3 -> Two hidden layer with 256 units')
parser.add_argument('-md', '--modelsDir', dest="modelsPath", default="mdls/",
                    help='Params file path with file name')
parser.add_argument('-p', '--params', dest="params", default="params.json",
                    help='Params file path with file name')

args = parser.parse_args()



json_file = open(args.modelsPath+args.params, "r")
params_str = json_file.read()
json_file.close()
params = json.loads(params_str)
print(params)

#Load Dataset
bin_data=[]
with h5py.File(os.path.join(args.inputDir,args.inputFile),'r') as f:
    data = f.get('Sb') 
    bin_data = np.array(data, dtype=np.bool) # For converting to numpy array
dim_set = len(bin_data)
print(dim_set)
print(bin_data)
        

#Foreach models
for model in args.models:
    #Load models from json
    if(model == 1):
        json_file = open(args.modelsPath+"json/NN0.json", "r")
        loaded_model = json_file.read()
        json_file.close()

        nn_model = tf.keras.models.model_from_json(loaded_model)
        nn_model.build()
        nn_model.summary()
        nn_name="NN0"
        nn_model.load_weights(args.outputPath+"/"+params["chk-dir"]+"/"+nn_name+"/best_model.h5py")

    elif(model == 2):
        json_file = open(args.modelsPath+"json/NN1.json", "r")
        loaded_model = json_file.read()
        json_file.close()

        nn_model = tf.keras.models.model_from_json(loaded_model)
        nn_model.build()
        nn_model.summary()
        nn_name="NN1"
        nn_model.load_weights(args.outputPath+"/"+params["chk-dir"]+"/"+nn_name+"/best_model.h5py")


    elif(model==3):
        json_file = open(args.modelsPath+"json/NN2.json", "r")
        loaded_model = json_file.read()
        json_file.close()

        nn_model = tf.keras.models.model_from_json(loaded_model)
        nn_model.build()
        nn_model.summary()
        nn_name="NN2"
        nn_model.load_weights(args.outputPath+"/"+params["chk-dir"]+"/"+nn_name+"/best_model.h5py")
    start = time.time()
    nn_model.predict(x=bin_data)
    end = time.time()

    elapsed = end - start

    if(not os.path.exists(os.path.join(args.outputPath, "predictionTime.csv"))):
        with open(os.path.join(args.outputPath, "predictionTime.csv"), "w") as fp:
            fp.write("Filename, # Elements ,NN Model, Predictions Time (s)\n")

    with open(os.path.join(args.outputPath, "predictionTime.csv"), "a+") as fp:
        fp.write(args.inputFile+", "+str(dim_set)+', '+nn_name+", "+str(elapsed)+"\n")
