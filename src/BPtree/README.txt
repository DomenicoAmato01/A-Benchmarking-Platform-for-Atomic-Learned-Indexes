
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


Per eseguire il codice:

cmake -DCMAKE_CXX_COMPILER=/usr/bin/g++-7 CMakeLists.txt
make
