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
