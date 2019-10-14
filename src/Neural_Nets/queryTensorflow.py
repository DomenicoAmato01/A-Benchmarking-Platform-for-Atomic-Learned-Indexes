import argparse
import json
import pickle
import os
import h5py
import tensorflow as tf
import numpy as np


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
parser.add_argument('-i', '--input', dest="inputFile", default="rsc/uniform/",
                    help='Input file name')
parser.add_argument('-id', '--inputDir', dest="inputDir", default="uni01.query10.bin.h5",
help='Input file path')
parser.add_argument('-o', '--output', dest="outputPath", default="res/uniform/query",
                    help='Output file path')
parser.add_argument('-m', '--model', dest="models", nargs="+", default="1", type=int, choices=range(1, 4),
                    help='Model numbers: 1 -> No hidden Layer(Perceptron); 2 -> One hidden layer with 256 units; 3 -> Two hidden layer with 256 units')
parser.add_argument('-md', '--modelsDir', dest="modelsPath", default="mdls/",
                    help='Params file path with file name')
parser.add_argument('-p', '--params', dest="params", default="params.json",
                    help='Params file path with file name')
#parser.add_argument('-perc', '--percentage', dest="percList", nargs="+", default="100",
#                    help='Params file path with file name')

args = parser.parse_args()



json_file = open(args.modelsPath+args.params, "r")
params_str = json_file.read()
json_file.close()
params = json.loads(params_str)
print(params)

#Load Dataset
bin_data=[]
with h5py.File(args.inputDir+"/"+args.inputFile,'r') as f:
    data = f.get('Sb') 
    bin_data = np.array(data, dtype=np.bool) # For converting to numpy array
bin_data = np.transpose(bin_data)
bin_data = np.flip(bin_data,axis=1)
dim_set = len(bin_data)
print(dim_set)
print(bin_data)
        

labels = np.linspace(1, len(bin_data), num=len(bin_data), dtype=np.float64)
labels = labels/len(bin_data)
labels = np.reshape(labels, (-1, 1))

#Foreach models
for model in args.models:
    #Load models from json
    if(model == 1):
        json_file = open(args.modelsPath+"json/NN1.json", "r")
        loaded_model = json_file.read()
        json_file.close()

        nn_model = tf.keras.models.model_from_json(loaded_model)
        nn_model.build()
        nn_model.summary()
        nn_name="NN1"
        nn_model.load_weights(args.outputPath+"/"+params["chk-dir"]+"/"+nn_name+"/best_model.h5py")

    elif(model == 2):
        json_file = open(args.modelsPath+"json/NN2.json", "r")
        loaded_model = json_file.read()
        json_file.close()

        nn_model = tf.keras.models.model_from_json(loaded_model)
        nn_model.build()
        nn_model.summary()
        nn_name="NN2"
        nn_model.load_weights(args.outputPath+"/"+params["chk-dir"]+"/"+nn_name+"/best_model.h5py")


    elif(model==3):
        json_file = open(args.modelsPath+"json/NN3.json", "r")
        loaded_model = json_file.read()
        json_file.close()

        nn_model = tf.keras.models.model_from_json(loaded_model)
        nn_model.build()
        nn_model.summary()
        nn_name="NN3"
        nn_model.load_weights(args.outputPath+"/"+params["chk-dir"]+"/"+nn_name+"/best_model.h5py")

'''
        labels = []
        with h5py.File('./Resource/'+distr+'/Query/file'+str(i)+distr+'Query'+perc+'_bin.mat','r') as f:
            data = f.get('Sb') 
            bin_data = np.array(data, dtype=np.bool) # For converting to numpy array
        bin_data = np.transpose(bin_data)
        bin_data = np.flip(bin_data,axis=1)
        dim_set = len(bin_data)
        labels = np.linspace(1, len(bin_data), num=len(bin_data), dtype=np.float64)
        labels = labels/len(bin_data)
        labels = np.reshape(labels, (-1, 1))


        result1, predTime1 = kerasModel1.evaluate(data_query, labels_query)
        result2, predTime2 = kerasModel2.evaluate(data_query, labels_query)
        result3, predTime3 = kerasModel3.evaluate(data_query, labels_query)
        
        real = np.multiply(labels_query,dim_set)

        pred1 = kerasModel1.predict(data_query)
        error1 = np.abs(pred1[0] - real)
        maxerr1 = np.max(error1)
        meanerr1 = np.mean(error1)

        pred2 = kerasModel2.predict(data_query)
        error2 = np.abs(pred2[0] - real)
        maxerr2 = np.max(error2)
        meanerr2 = np.mean(error2)

        pred3 = kerasModel3.predict(data_query)
        error3 = np.abs(pred3[0] - real)
        maxerr3 = np.max(error3)
        meanerr3 = np.mean(error3)

        result1[3] = meanerr1
        result2[3] = meanerr2
        result3[3] = meanerr3
        result1[4] = maxerr1
        result2[4] = maxerr2
        result3[4] = maxerr3

        result.append([(result1, predTime1),( result2, predTime2),(result3, predTime3)])
        
    epsilon = [np.ceil(result1[4]), np.ceil(result2[4]), np.ceil(result3[4])]


    with open('./Result/'+distr+'/file'+str(i)+'/'+dirExpName+'/query_eval.pickle','wb') as fp:
        pickle.dump(result, fp)

    with open('./Result/'+distr+'/file'+str(i)+'/'+dirExpName+'/query_epsilon.pickle','wb') as fp:
        pickle.dump(epsilon, fp)
'''