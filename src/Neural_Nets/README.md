## Neural Networks
### Transforming Datasets
In order to train Neural Nets and querying data, csv files, containing one element for each column, must be converted in hdf5 files containing their 64-bit binary rappresentation.

```
python [sourcePath]/createBinArray.py [-h] [-i INPUTFILE] [-id INPUTDIR] [-o OUTPUTPATH]


optional arguments:\n
  -h, --help            show this help message and exit  
  -i INPUTFILE, --input INPUTFILE  
                        Input file name   
                        default = 'uni01.sorted.csv'  
  -id INPUTDIR, --inputDir INPUTDIR  
                        Input file path  
                        deafult = 'rsc/uniform/'  
  -o OUTPUTPATH, --output OUTPUTPATH  
                        Output file path and file name  
                        default = 'rsc/uniform/uni01.sorted.bin.h5'  
```

### Train NN

Training is written in python using Tensorflow Keras high-level API as neural nets implementation.  
There are 3 kind of [models](mdls/json/):
1. NN0 is a Fully connected net with no hidden layers
2. NN1 is a fully connected net with 1 hidden layer composed by 256 units
3. NN2 is a fully connected net with 2 hidden layers composed by 256 units each one

To run the script use:  


```
python [sourcePath]/trainNN.py [-h] [-i INPUTFILE] [-id INPUTDIR] [-o OUTPUTPATH] [-m {1,2,3} [{1,2,3} ...]] [-p PARAMS]


optional arguments:
  -h, --help            show this help message and exit
  -i INPUTFILE, --input INPUTFILE
                        Input file name
                        default = 'uni01.sorted.bin.h5'
  -id INPUTDIR, --inputDir INPUTDIR
                        Input file path
                        default = 'rsc/uniform/'
  -o OUTPUTPATH, --output OUTPUTPATH
                        Output file path
                        default = 'res/uniform/'
  -m {1,2,3} [{1,2,3} ...], --model {1,2,3} [{1,2,3} ...]
                        Model numbers: 1 -> No hidden Layer(Perceptron); 2 ->
                        One hidden layer with 256 units; 3 -> Two hidden layer
                        with 256 units
                        default=1
  -p PARAMS, --params PARAMS
                        Params file path and file name 
                        default='mdls/params.json'
```

Multiple params '-m' 

### Query Scripts

Querying are performed with four different methods to compare different implementation performance.

#### Tensorflow API

The scripts was written in python using Tensorflow Keras high-level API as in the Training Phase.  

```
queryTensorflow.py  [-h] [-i INPUTFILE] [-id INPUTDIR] [-o OUTPUTPATH]
                    [-m {1,2,3} [{1,2,3} ...]] [-md MODELSPATH]
                    [-p PARAMS]

optional arguments:
  -h, --help            show this help message and exit
  -i INPUTFILE, --input INPUTFILE
                        Input file name
                        default = 'uni01.Query10.bin.h5'
  -id INPUTDIR, --inputDir INPUTDIR
                        Input file path
                        default = 'rsc/uniform/Query'
  -o OUTPUTPATH, --output OUTPUTPATH
                        Output file path
                        default = 'res/uniform/'
  -m {1,2,3} [{1,2,3} ...], --model {1,2,3} [{1,2,3} ...]
                        Model numbers: 1 -> No hidden Layer(Perceptron); 2 ->
                        One hidden layer with 256 units; 3 -> Two hidden layer
                        with 256 units
                        default = 1
  -md MODELSPATH, --modelsDir MODELSPATH
                        Params file path with file name
                        default = 'mdls/'
  -p PARAMS, --params PARAMS
                        Params file path with file name
                        default = 'params.json'
```

#### Cuda GPU

#### Cuda TPU

#### Intel CPU

The scripts was written in C++ and use the intel Math Kernel Library.  
Requirements are:
* Intel CPU
* g++ compiler version 7 or later
* Intel mkl library installed

To compile use :

```

g++ queryCPU.cpp -o [scriptName] -Wl,--start-group [mkl_dir]/libmkl_intel_lp64.a [mkl_dir]/libmkl_sequential.a [mkl_dir]/libmkl_core.a -Wl,--end-group -lpthread -lm -ldl

```

To run the compiled script use;

```

./[scriptName] -d dataName -q queryName -o outputName [-h]

```

For example:

./queryCPU -d uni01 -q 10 -o predictionTime 

search in the current directory a file with name uni01Query10_bin.dat and produce a csv file named predictionTime.csv  
  
Output file structures is:

```

Dataset, Query, #Elements, Mult Time(s), Bias Time (s), Relu Time(s), Tot Time(s), Mean Time(s)

```

where

|     Field          |      Description         |
| ------------- | ------------- |
| Dataset       | Dataset Name  |
| Query         | Query Name    |
| #Elements      | Number of Elements in Query    |
| Mult Time(s)  | Time to perform Matrix Multiplication expressed in seconds    |
| Bias Time(s)  | Time to perform Bias Sum expressed in seconds    |
| Relu Time(s)  | Time to perform Relu Activation Layers expressed in seconds    |
| Tot Time(s)   | Sum of the time below    |
| Mean Time(s)  | Tot Time / #Elements   |


