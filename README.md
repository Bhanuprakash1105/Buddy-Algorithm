# Buddy-Algorithm

## Concept

The buddy system is a memory allocation and management algorithm that manages memory. In a buddy system, the entire memory space available for allocation is initially treated as a single block whose size is a power of 2.

When the first request is made, if its size is greater than half of the initial block then the entire block is allocated.
Otherwise, the block is split in two equal companion buddies. If the size of the request is greater than half of one of the buddies, then allocate one to it.
Otherwise, one of the buddies is split in half again.
_This method continues until the smallest block greater than or equal to the size of the request is found and allocated to it._

In this method, when a memory block that was allocated is freed then if possible, an unnallocated buddy is merged with a companion buddy in order to form a larger free block.
_Two blocks are said to be companion buddies if they resulted from the split of the same direct parent block._

## Running the program

The algorithm is implemented in C++ programming language (_Latest g++ compiler is recommended_)

Menu driven code with clean statements for user inputs

Follow the code and refer the sample terminal screenshots provided in the repository
