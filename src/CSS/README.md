
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
