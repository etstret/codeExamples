# codeExamples

## GPU Parallelisation
This has two elements, a non-kernel file written in C++ that computes all the necessary, non-parallelised aspects (data I/O, kernel setup and execution, kernel runtime calculations etc.) and a kernel file written in OpenCL that actually performs the parallelised sums etc.
