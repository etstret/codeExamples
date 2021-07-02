# codeExamples

## GPU Parallelisation
This has two elements, a non-kernel file written in C++ that computes all the necessary, non-parallelised aspects (data I/O, kernel setup and execution, kernel runtime calculations etc.) and a kernel file written in OpenCL that actually performs the parallelised sums etc.

## Shapes
This was a University-driven project in order to practise and refine the concepts of encapsulation, inheritence and polymorphism. Driver.cpp contains the main function, and the program effectively recalculates aspects of shapes (corner coordinates, circle radius etc.) when given particular points from the user.
