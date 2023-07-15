# CACHE Simulator

## Description
This is a cache simulator that demonstrates how the memory access through cache. 

## Technology Stack
- C: Cache Simulator is implemented using C
- Cache Memory (Refer [this](https://www.cs.cmu.edu/afs/cs/academic/class/15213-f09/www/lectures/23-caches.pdf) to brush up your understanding of cache)
- Cache Structure: Tag, Set Number, Block Offset
- Placement Policy: Direct Cache Mapping
- Replacement Policy: Least Recently Used (LRU)

## Usage    
Specify the input in the following format  
`./cache    [-hv]    -s    <s>    -E    <E>    -b    <b>    -t    <tracefile>`

|Parameter        |Functionality|
|:----------------|:-----------------------------------------------|
|-h               | Optional help ﬂag that prints usage info |
|-v               | Optional verbose ﬂag that displays trace info |  
|-s  \<s>         | Number of set index bits (S = 2^s is the number of sets) |  
|-E  \<E>         | Associativity (number of lines per set) |
|-b  \<b>         | Number of block bits (B = 2^b is the block size) |   
|-t  \<tracefile> | Name/Path of the memory trace |

HIT is when the cache memory access is successful, that is the tag match and block is available  
MISS is when the address isn't available in cache. The address is added to the cache  

## Files:
cache.c &nbsp; &nbsp; &nbsp; Source Code  
README &nbsp; &nbsp; This file  
traces/ &nbsp;&nbsp; &nbsp; &nbsp; Contains trace files used for cache simulator  
