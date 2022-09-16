# Cuda practice

## Compile

`nvcc -std=c++11 src/main.cu`   

## Run

`./a.out <kernel_idx> (big | small)` - where kernel_idx in {0,1,2}   
0 - gaussian 3x3   
1 - gaussian 5x5   
2 - edge detect 3x3   
