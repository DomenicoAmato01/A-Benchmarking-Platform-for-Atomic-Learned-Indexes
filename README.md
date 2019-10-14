# Benchmarking Platform for Index Neural Nets

## Directory Structure
    .
    ├── src                    # Source Directory
    |   ├── Neural_Nets        # Neural Nets Scripts          
    │   └── Paco-Lcp           # Paco Lcp
    ├── rsc                    # Resource Directory
    │   └── uniform            # Uniform Dataset
    |── res                    # Results Directory
    |   └──uniform             # Uniform Dataset
    └── mdls                   # Models Directory   
        └──json                # Neural Nets Json 
    
## Neural Networks
### Create Datasets
### Train NN
### Query Scripts

## Standard Data Structures
### Paco
An order-preserving minimal perfect hash function , that maps bijectively a set of ordered keys to their ordinal position position in a given list. It assumes that keys to be hashed are ordered. The [source code](src/Paco-Lcp/paco) is written in Java and it exploits the provided [libraries](src/Paco-Lcp/lib).

> Creation & Query
```
java -cp ./lib paco.PACO -f dataset.csv [-q queryFile]
```

### LCP 
A monotone minimal perfect hash function that map the keys of a lexicographically sorted set to its ordinal position. It assumes that keys to be hashed are ordered. The [source code](src/Paco-Lcp/lcp) is written in Java and it exploits the provided [libraries](src/Paco-Lcp/lib).

> Creation & Query
```
java -cp ./lib lcp.LCP -f dataset.csv [-q queryFile]
```

### B+tree
### CSS Tree
### Self Adjusting Binary Tree
