# A Benchmarking Platform for Atomic Learned Indexes

This repository provides a benchmarking platform to evaluate how Feed Forward Neural Networks can be effectively used as index data structures. That is, how well in terms of time and space, those Networks can solve the well known predecessor problem, i.e., searching for an element in a sorted table. In theory, those networks, with RELU activators,  are able to approximate any function [ ] , including the one needed to solve predecessor search. Then, a simple binary search concludes the task.  
  
The scenario we have considered is in memory data and batch queries, which is a fundamental one, as outlined in [ CSS].  
  
We include  
  
—three neural networks…  
-strutture dati …  
-scripting to execute the code provided here in different scenarios, including GPU Cuda Cores Tensor Core.  
  
  
The datsets used for benchmarking are in ….   
  
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
