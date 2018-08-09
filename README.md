---
title: "DeltaQuickReduct"
author: "Javad Rahimipour Anaraki"
date: '08/02/18'
---

## Use case
To determine the most important features using the algorithm described in "A New Fuzzy-Rough Hybrid Merit to Feature Selection" by Javad Rahimipour Anaraki, Saeed Samet, Wolfgang Banzhaf and Mahdi Eftekhari

Here is a link to the paper: https://link.springer.com/chapter/10.1007/978-3-662-53611-7_1

## Compile
To compile this program follow these steps:

1. Be sure that you have the latest [GCC/G++ compiler](https://gcc.gnu.org/) installed
2. Use `g++ -o DQR DQR.cpp -std=c++11` to compile the program
3. To improve its performance one can use `-O1` or `-O2` or `-O3`
4. Ignore the following warning message:

        DQR.cpp:238:14: warning: expression result unused [-Wunused-value]
        for (s;s<cls[nCls];++s) {
        ^
        1 warning generated.

## Run
To run the program use `./DGR {a dataset name}`

## Note
The classification outcome column of dataset should be sorted ascendingly
