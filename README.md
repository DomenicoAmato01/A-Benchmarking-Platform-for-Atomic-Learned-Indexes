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
    └── src                    # Source Directory
        ├── Neural_Nets           # Neural Nets Scripts Directory
              └── mdls                # Models Directory   
                    └──json               # Neural Nets Json
        └── Data_Structures    # Data Structures Main Directory
              ├── Paco-Lcp        # Paco Lcp
              ├── BPtree          # B+Tree
              ├── CSS             # CSS Tree
              └── SABT            # Self Adjusting Binary Tree
     


