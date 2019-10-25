# Benchmarking Platform for Index Neural Nets

## Directory Structure
    .
    ├── src                    # Source Directory
    |   ├── Neural_Nets        # Neural Nets Scripts          
    │   ├── Paco-Lcp           # Paco Lcp
    │   ├── BPtree             # B+Tree
    │	  ├── CSS                # CSS Tree
    │   └── SABT               # Self Adjusting Binary Tree
    ├── rsc                    # Resource Directory
    │   └── uniform            # Uniform Dataset
    |── res                    # Results Directory
    |   └──uniform             # Uniform Dataset
    └── mdls                   # Models Directory   
        └──json                # Neural Nets Json 
    
## Neural Networks
### Create Datasets
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


## Standard Data Structures
### Paco
An order-preserving minimal perfect hash function , that maps bijectively a set of ordered keys to their ordinal position position in a given list. It assumes that keys to be hashed are ordered. The [source code](src/Paco-Lcp/paco) is written in Java and it exploits the provided [libraries](src/Paco-Lcp/lib).

> Creation 
```
java -cp ./lib paco.PACO -f PATH_TO_DATASET_FILE

arguments:
  -f PATH_TO_DATASET_FILE 
  		Path to the dataset file
```

> Query
```
java -cp ./lib paco.PACO -f PATH_TO_DATASET_FILE -q PATH_TO_QUERY_FILE

arguments:
  -f PATH_TO_DATASET_FILE 
  		Path to the csv dataset file
  -q PATH_TO_QUERY_FILE 
  		Path to the csv query dataset file
```

### LCP 
A monotone minimal perfect hash function that map the keys of a lexicographically sorted set to its ordinal position. It assumes that keys to be hashed are ordered. The [source code](src/Paco-Lcp/lcp) is written in Java and it exploits the provided [libraries](src/Paco-Lcp/lib).

> Creation
```
java -cp ./lib lcp.LCP -f PATH_TO_DATASET_FILE 
arguments:
  -f PATH_TO_DATASET_FILE 
  		Path to the dataset file
```

> Query
```
java -cp ./lib lcp.LCP -f PATH_TO_DATASET_FILE -q PATH_TO_QUERY_FILE
  -f PATH_TO_DATASET_FILE 
  		Path to the csv dataset file
  -q PATH_TO_QUERY_FILE 
  		Path to the csv query dataset file
```

### B+tree
A variants of the B-tree used for indexing a set of keys to the corresponding data where all leaves of the tree appear at the same
level and they are organized as a doubly linked list.  The [source code](src/BPtree) is written in C++ and it exploits the provided [libraries](src/BPtree/stx). In order to create executables use the following commands: 

```
cmake -DCMAKE_CXX_COMPILER=/usr/bin/g++-7 CMakeLists.txt
make
```

> Creation
```
./IndexBtree256 PATH_TO_DATASET_FILE   # For page block of 256 bytes
./IndexBtree8192 PATH_TO_DATASET_FILE   # For page block of 8192 bytes
arguments:
  PATH_TO_DATASET_FILE 
  		Path to the dataset file
```

> Query
```
./IndexBtree256 PATH_TO_DATASET_FILE PATH_TO_QUERY_FILE  # For page block of 256 bytes
./IndexBtree8192 PATH_TO_DATASET_FILE PATH_TO_QUERY_FILE  # For page block of 8192 bytes
arguments:
  PATH_TO_DATASET_FILE 
  		Path to the csv dataset file
  PATH_TO_QUERY_FILE 
  		Path to the csv query dataset file
```

### CSS Tree
Cache-Sensitive Search tree (CSS-tree) which exploits search tree where all the leaves of the tree correspond to blocks of consecutive keys.
The [source code](src/CSS) is written in C++ and it exploits the provided [libraries](src/CSS/stx). In order to create executables use the following commands: 

```
cmake -DCMAKE_CXX_COMPILER=/usr/bin/g++-7 CMakeLists.txt
make
```

> Creation
```
./IndexCSS256 PATH_TO_DATASET_FILE   # For page block of 256 bytes
./IndexCSS8192 PATH_TO_DATASET_FILE   # For page block of 8192 bytes
arguments:
  PATH_TO_DATASET_FILE 
  		Path to the dataset file
```

> Query
```
./IndexCSS256 PATH_TO_DATASET_FILE PATH_TO_QUERY_FILE  # For page block of 256 bytes
./IndexCSS8192 PATH_TO_DATASET_FILE PATH_TO_QUERY_FILE  # For page block of 8192 bytes
arguments:
  PATH_TO_DATASET_FILE 
  		Path to the csv dataset file
  PATH_TO_QUERY_FILE 
  		Path to the csv query dataset file
```

### Self Adjusting Binary Tree
A type of balanced binary search tree for storing an ordered set of keys where the tree structure is organized over time as keys are inserted, accessed, or removed. The [source code](src/SABT) is written in Java.

> Creation
```
java SABT.SplayTree PATH_TO_DATASET_FILE 
arguments:
  PATH_TO_DATASET_FILE 
  		Path to the dataset file
```

> Query
```
java SABT.SplayTree PATH_TO_DATASET_FILE PATH_TO_QUERY_FILE
  PATH_TO_DATASET_FILE 
  		Path to the csv dataset file
  PATH_TO_QUERY_FILE 
  		Path to the csv query dataset file
```